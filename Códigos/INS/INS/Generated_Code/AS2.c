/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : AS2.c
**     Project     : INS
**     Processor   : MCF51QE64CLH
**     Component   : AsynchroSerial
**     Version     : Component 02.611, Driver 01.33, CPU db: 3.00.080
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2016-09-09, 04:38, # CodeGen: 1
**     Abstract    :
**         This component "AsynchroSerial" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial channel.
**     Settings    :
**         Serial channel              : SCI2
**
**         Protocol
**             Init baud rate          : 9600baud
**             Width                   : 8 bits
**             Stop bits               : 1
**             Parity                  : none
**             Breaks                  : Disabled
**             Input buffer size       : 500
**             Output buffer size      : 500
**
**         Registers
**             Input buffer            : SCI2D     [0xFFFF9877]
**             Output buffer           : SCI2D     [0xFFFF9877]
**             Control register        : SCI2C1    [0xFFFF9872]
**             Mode register           : SCI2C2    [0xFFFF9873]
**             Baud setting reg.       : SCI2BD    [0xFFFF9870]
**             Special register        : SCI2S1    [0xFFFF9874]
**
**         Input interrupt
**             Vector name             : Vsci2rx
**             Priority                : 240
**
**         Output interrupt
**             Vector name             : Vsci2tx
**             Priority                : 240
**
**         Used pins:
**         ----------------------------------------------------------
**           Function | On package           |    Name
**         ----------------------------------------------------------
**            Input   |     52               |  PTC6_RGPIO14_RxD2_ACMP2PLUS
**            Output  |     51               |  PTC7_RGPIO15_TxD2_ACMP2MINUS
**         ----------------------------------------------------------
**
**
**
**     Contents    :
**         RecvChar        - byte AS2_RecvChar(AS2_TComData *Chr);
**         SendChar        - byte AS2_SendChar(AS2_TComData Chr);
**         RecvBlock       - byte AS2_RecvBlock(AS2_TComData *Ptr, word Size, word *Rcv);
**         SendBlock       - byte AS2_SendBlock(AS2_TComData *Ptr, word Size, word *Snd);
**         ClearRxBuf      - byte AS2_ClearRxBuf(void);
**         ClearTxBuf      - byte AS2_ClearTxBuf(void);
**         GetCharsInRxBuf - word AS2_GetCharsInRxBuf(void);
**         GetCharsInTxBuf - word AS2_GetCharsInTxBuf(void);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file AS2.c
** @version 01.33
** @brief
**         This component "AsynchroSerial" implements an asynchronous serial
**         communication. The component supports different settings of
**         parity, word width, stop-bit and communication speed,
**         user can select interrupt or polling handler.
**         Communication speed can be changed also in runtime.
**         The component requires one on-chip asynchronous serial channel.
*/         
/*!
**  @addtogroup AS2_module AS2 module documentation
**  @{
*/         


/* MODULE AS2. */

#include "AS2.h"
#include "Events.h"



/* SerFlag bits */
#define OVERRUN_ERR      0x01U         /* Overrun error flag bit   */
#define COMMON_ERR       0x02U         /* Common error of RX       */
#define CHAR_IN_RX       0x04U         /* Char is in RX buffer     */
#define RUNINT_FROM_TX   0x08U         /* Interrupt is in progress */
#define FULL_RX          0x10U         /* Full receive buffer      */

static volatile byte SerFlag;          /* Flags for serial communication */
                                       /* Bit 0 - Overrun error */
                                       /* Bit 1 - Common error of RX */
                                       /* Bit 2 - Char in RX buffer */
                                       /* Bit 3 - Interrupt is in progress */
                                       /* Bit 4 - Full RX buffer */
word AS2_InpLen;                       /* Length of the input buffer content */
static word InpIndxR;                  /* Index for reading from input buffer */
static word InpIndxW;                  /* Index for writing to input buffer */
static AS2_TComData InpBuffer[AS2_INP_BUF_SIZE]; /* Input buffer for SCI commmunication */
word AS2_OutLen;                       /* Length of the output buffer content */
static word OutIndxR;                  /* Index for reading from output buffer */
static word OutIndxW;                  /* Index for writing to output buffer */
static AS2_TComData OutBuffer[AS2_OUT_BUF_SIZE]; /* Output buffer for SCI commmunication */
static bool OnFreeTxBuf_semaphore;     /* Disable the false calling of the OnFreeTxBuf event */

/*
** ===================================================================
**     Method      :  AS2_RecvChar (component AsynchroSerial)
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). This method is enabled only if the receiver property
**         is enabled.
**         [Note:] Because the preferred method to handle error and
**         break exception in the interrupt mode is to use events
**         <OnError> and <OnBreak> the return value ERR_RXEMPTY has
**         higher priority than other error codes. As a consequence the
**         information about an exception in interrupt mode is returned
**         only if there is a valid character ready to be read.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to a received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_BREAK - Break character is detected
**                           (only when the <Interrupt service> property
**                           is disabled and the <Break signal> property
**                           is enabled)
**                           ERR_COMMON - common error occurred (the
**                           <GetError> method can be used for error
**                           specification)
** ===================================================================
*/
byte AS2_RecvChar(AS2_TComData *Chr)
{
  byte Result = ERR_OK;                /* Prepare default error code */

  if (AS2_InpLen > 0U) {               /* Is number of received chars greater than 0? */
    EnterCritical();                   /* Save the PS register */
    AS2_InpLen--;                      /* Decrease number of received chars */
    *Chr = InpBuffer[InpIndxR];        /* Received char */
    if (++InpIndxR >= AS2_INP_BUF_SIZE) { /* Is the index out of the buffer? */
      InpIndxR = 0U;                   /* Set the index to the start of the buffer */
    }
    Result = (byte)((SerFlag & (OVERRUN_ERR|COMMON_ERR|FULL_RX)) ? ERR_COMMON : ERR_OK);
    SerFlag &= (byte)(~(byte)(OVERRUN_ERR|COMMON_ERR|FULL_RX|CHAR_IN_RX)); /* Clear all errors in the status variable */
    ExitCritical();                    /* Restore the PS register */
  } else {
    return ERR_RXEMPTY;                /* Receiver is empty */
  }
  return Result;                       /* Return error code */
}

/*
** ===================================================================
**     Method      :  AS2_SendChar (component AsynchroSerial)
**     Description :
**         Sends one character to the channel. If the component is
**         temporarily disabled (Disable method) SendChar method only
**         stores data into an output buffer. In case of a zero output
**         buffer size, only one character can be stored. Enabling the
**         component (Enable method) starts the transmission of the
**         stored data. This method is available only if the
**         transmitter property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
byte AS2_SendChar(AS2_TComData Chr)
{
  if (AS2_OutLen == AS2_OUT_BUF_SIZE) { /* Is number of chars in buffer the same as a size of the transmit buffer */
    return ERR_TXFULL;                 /* If yes then error */
  }
  EnterCritical();                     /* Save the PS register */
  AS2_OutLen++;                        /* Increase number of bytes in the transmit buffer */
  OutBuffer[OutIndxW] = Chr;           /* Store char to buffer */
  if (++OutIndxW >= AS2_OUT_BUF_SIZE) { /* Is the index out of the buffer? */
    OutIndxW = 0U;                     /* Set the index to the start of the buffer */
  }
  if (SCI2C2_TIE == 0U) {              /* Is the transmit interrupt already enabled? */
    SCI2C2_TIE = 0x01U;                /* If no than enable transmit interrupt */
  }
  ExitCritical();                      /* Restore the PS register */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS2_RecvBlock (component AsynchroSerial)
**     Description :
**         If any data is received, this method returns the block of
**         the data and its length (and incidental error), otherwise it
**         returns an error code (it does not wait for data).
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is enabled.
**         If less than requested number of characters is received only
**         the available data is copied from the receive buffer to the
**         user specified destination. The value ERR_EXEMPTY is
**         returned and the value of variable pointed by the Rcv
**         parameter is set to the number of received characters.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of received data
**         Size            - Size of the block
**       * Rcv             - Pointer to real number of the received data
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - The receive buffer didn't
**                           contain the requested number of data. Only
**                           available data has been returned.
**                           ERR_COMMON - common error occurred (the
**                           GetError method can be used for error
**                           specification)
** ===================================================================
*/
byte AS2_RecvBlock(AS2_TComData *Ptr, word Size, word *Rcv)
{
  word count;                          /* Number of received chars */
  byte result = ERR_OK;                /* Last error */

  for (count = 0U; count < Size; count++) {
    switch (AS2_RecvChar(Ptr++)) {     /* Receive data and test the return value*/
    case ERR_RXEMPTY:                  /* No data in the buffer */
      if (result == ERR_OK) {          /* If no receiver error reported */
        result = ERR_RXEMPTY;          /* Return info that requested number of data is not available */
      }
     *Rcv = count;                     /* Return number of received chars */
      return result;
    case ERR_COMMON:                   /* Receiver error reported */
      result = ERR_COMMON;             /* Return info that an error was detected */
      break;
    default:
      break;
    }
  }
  *Rcv = count;                        /* Return the number of received chars */
  return result;                       /* Return the last error code*/
}

/*
** ===================================================================
**     Method      :  AS2_SendBlock (component AsynchroSerial)
**     Description :
**         Sends a block of characters to the channel.
**         This method is available only if non-zero length of the
**         output buffer is defined and the transmitter property is
**         enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of data to send
**         Size            - Size of the block
**       * Snd             - Pointer to number of data that are sent
**                           (moved to buffer)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - It was not possible to send
**                           requested number of bytes
** ===================================================================
*/
byte AS2_SendBlock(const AS2_TComData * Ptr, word Size, word *Snd)
{
  word count = 0x00U;                  /* Number of sent chars */
  bool local_OnFreeTxBuf_semaphore = OnFreeTxBuf_semaphore; /* Local copy of OnFreeTxBuf_semaphore state */

  while((count < Size) && (AS2_OutLen < AS2_OUT_BUF_SIZE)) { /* While there is some char desired to send left and output buffer is not full do */
    EnterCritical();                   /* Save the PS register */
    OnFreeTxBuf_semaphore = TRUE;      /* Set the OnFreeTxBuf_semaphore to block OnFreeTxBuf calling */
    AS2_OutLen++;                      /* Increase number of bytes in the transmit buffer */
    OutBuffer[OutIndxW] = *Ptr++;      /* Store char to buffer */
    if (++OutIndxW >= AS2_OUT_BUF_SIZE) { /* Is the index out of the buffer? */
      OutIndxW = 0U;                   /* Set the index to the start of the buffer */
    }
    count++;                           /* Increase the count of sent data */
    if ((count == Size) || (AS2_OutLen == AS2_OUT_BUF_SIZE)) { /* Is the last desired char put into buffer or the buffer is full? */
      if (!local_OnFreeTxBuf_semaphore) { /* Was the OnFreeTxBuf_semaphore clear before enter the method? */
        OnFreeTxBuf_semaphore = FALSE; /* If yes then clear the OnFreeTxBuf_semaphore */
      }
    }
    if (SCI2C2_TIE == 0U) {            /* Is the transmit interrupt already enabled? */
      SCI2C2_TIE = 0x01U;              /* If no than enable transmit interrupt */
    }
    ExitCritical();                    /* Restore the PS register */
  }
  *Snd = count;                        /* Return the number of sent chars */
  if (count != Size) {                 /* Is the number of sent chars less then desired number of chars */
    return ERR_TXFULL;                 /* If yes then error */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS2_ClearRxBuf (component AsynchroSerial)
**     Description :
**         Clears the receive buffer.
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte AS2_ClearRxBuf(void)
{
  EnterCritical();                     /* Save the PS register */
  AS2_InpLen = 0x00U;                  /* Set number of chars in the receive buffer to 0 */
  InpIndxR = 0x00U;                    /* Reset read index to the receive buffer */
  InpIndxW = 0x00U;                    /* Reset write index to the receive buffer */
  SerFlag &= (byte)(~(byte)(CHAR_IN_RX | FULL_RX)); /* Clear the flags indicating a char in buffer */
  ExitCritical();                      /* Restore the PS register */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS2_ClearTxBuf (component AsynchroSerial)
**     Description :
**         Clears the transmit buffer.
**         This method is available only if non-zero length of the
**         output buffer is defined and the receiver property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte AS2_ClearTxBuf(void)
{
  EnterCritical();                     /* Save the PS register */
  AS2_OutLen = 0x00U;                  /* Set number of chars in the transmit buffer to 0 */
  OutIndxR = 0x00U;                    /* Reset read index to the transmit buffer */
  OutIndxW = 0x00U;                    /* Reset read index to the transmit buffer */
  ExitCritical();                      /* Restore the PS register */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AS2_GetCharsInRxBuf (component AsynchroSerial)
**     Description :
**         Returns the number of characters in the input buffer. This
**         method is available only if the receiver property is enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the input
**                           buffer.
** ===================================================================
*/
/*
word AS2_GetCharsInRxBuf(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  AS2_GetCharsInTxBuf (component AsynchroSerial)
**     Description :
**         Returns the number of characters in the output buffer. This
**         method is available only if the transmitter property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - The number of characters in the output
**                           buffer.
** ===================================================================
*/
/*
word AS2_GetCharsInTxBuf(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  AS2_InterruptRx (component AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the component's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_ERROR        0x01U
#define ON_FULL_RX      0x02U
#define ON_RX_CHAR      0x04U
#define ON_IDLE_CHAR    0x08U
#define ON_RX_CHAR_EXT  0x10U
ISR(AS2_InterruptRx)
{
  byte OnFlags = 0x00U;                /* Temporary variable for flags */
  byte StatReg = SCI2S1;               /* Temporary variable for status flags */
  AS2_TComData Data = SCI2D;           /* Read data from the receiver into temporary variable for data */

  if (AS2_InpLen < AS2_INP_BUF_SIZE) { /* Is number of bytes in the receive buffer lower than size of buffer? */
    AS2_InpLen++;                      /* Increse number of chars in the receive buffer */
    InpBuffer[InpIndxW] = Data;        /* Save received char to the receive buffer */
    if (++InpIndxW >= AS2_INP_BUF_SIZE) { /* Is the index out of the buffer? */
      InpIndxW = 0U;                   /* Set the index to the start of the buffer */
    }
    OnFlags |= ON_RX_CHAR;             /* Set flag "OnRXChar" */
    if (AS2_InpLen== AS2_INP_BUF_SIZE) { /* Is number of bytes in the receive buffer equal as a size of buffer? */
      OnFlags |= ON_FULL_RX;           /* If yes then set flag "OnFullRxBuff" */
    }
  } else {
    SerFlag |= FULL_RX;                /* If yes then set flag buffer overflow */
    OnFlags |= ON_ERROR;               /* Set flag "OnError" */
  }
  if (OnFlags & ON_ERROR) {            /* Is OnError flag set? */
    AS2_OnError();                     /* If yes then invoke user event */
  }
  else {
    if (OnFlags & ON_RX_CHAR) {        /* Is OnRxChar flag set? */
      AS2_OnRxChar();                  /* If yes then invoke user event */
    }
    if (OnFlags & ON_FULL_RX) {        /* Is OnFullRxBuf flag set? */
      AS2_OnFullRxBuf();               /* If yes then invoke user event */
    }
  }
}

/*
** ===================================================================
**     Method      :  AS2_InterruptTx (component AsynchroSerial)
**
**     Description :
**         The method services the transmit interrupt of the selected 
**         peripheral(s) and eventually invokes the component's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_FREE_TX  0x01U
#define ON_TX_CHAR  0x02U
ISR(AS2_InterruptTx)
{
  byte OnFlags = 0x00U;                /* Temporary variable for flags */

  if (SerFlag & RUNINT_FROM_TX) {      /* Is flag "running int from TX" set? */
    OnFlags |= ON_TX_CHAR;             /* Set flag "OnTxChar" */
  }
  SerFlag &= (byte)(~(byte)RUNINT_FROM_TX); /* Reset flag "running int from TX" */
  if (AS2_OutLen) {                    /* Is number of bytes in the transmit buffer greater than 0? */
    AS2_OutLen--;                      /* Decrease number of chars in the transmit buffer */
    SerFlag |= RUNINT_FROM_TX;         /* Set flag "running int from TX" */
    (void)SCI2S1;                      /* Reset interrupt request flag */
    SCI2D = OutBuffer[OutIndxR];       /* Store char to transmitter register */
    if (++OutIndxR >= AS2_OUT_BUF_SIZE) { /* Is the index out of the buffer? */
      OutIndxR = 0U;                   /* Set the index to the start of the buffer */
    }
  } else {
    if (!OnFreeTxBuf_semaphore) {
      OnFlags |= ON_FREE_TX;           /* Set flag "OnFreeTxBuf" */
    }
    SCI2C2_TIE = 0x00U;                /* Disable transmit interrupt */
  }
  if (OnFlags & ON_TX_CHAR) {          /* Is flag "OnTxChar" set? */
    AS2_OnTxChar();                    /* If yes then invoke user event */
  }
  if (OnFlags & ON_FREE_TX) {          /* Is flag "OnFreeTxBuf" set? */
    AS2_OnFreeTxBuf();                 /* If yes then invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  AS2_InterruptError (component AsynchroSerial)
**
**     Description :
**         The method services the error interrupt of the selected 
**         peripheral(s) and eventually invokes the component's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(AS2_InterruptError)
{
  byte StatReg = getReg(SCI2S1);

  (void)SCI2D;                         /* Dummy read of data register - clear error bits */
  SerFlag |= COMMON_ERR;               /* If yes then set an internal flag */
  AS2_OnError();                       /* Invoke user event */
}

/*
** ===================================================================
**     Method      :  AS2_Init (component AsynchroSerial)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AS2_Init(void)
{
  SerFlag = 0x00U;                     /* Reset flags */
  OnFreeTxBuf_semaphore = FALSE;       /* Clear the OnFreeTxBuf_semaphore */
  AS2_InpLen = 0x00U;                  /* No char in the receive buffer */
  InpIndxR = 0x00U;                    /* Reset read index to the receive buffer */
  InpIndxW = 0x00U;                    /* Reset write index to the receive buffer */
  AS2_OutLen = 0x00U;                  /* No char in the transmit buffer */
  OutIndxR = 0x00U;                    /* Reset read index to the transmit buffer */
  OutIndxW = 0x00U;                    /* Reset write index to the transmit buffer */
  /* SCI2C1: LOOPS=0,SCISWAI=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
  setReg8(SCI2C1, 0x00U);              /* Configure the SCI */ 
  /* SCI2C3: R8=0,T8=0,TXDIR=0,TXINV=0,ORIE=0,NEIE=0,FEIE=0,PEIE=0 */
  setReg8(SCI2C3, 0x00U);              /* Disable error interrupts */ 
  /* SCI2C2: TIE=0,TCIE=0,RIE=0,ILIE=0,TE=0,RE=0,RWU=0,SBK=0 */
  setReg8(SCI2C2, 0x00U);              /* Disable all interrupts */ 
  /* SCI2S2: LBKDIF=0,RXEDGIF=0,??=0,RXINV=0,RWUID=0,BRK13=0,LBKDE=0,RAF=0 */
  setReg8(SCI2S2, 0x00U);               
  SCI2BDH = 0x00U;                     /* Set high divisor register (enable device) */
  SCI2BDL = 0xA4U;                     /* Set low divisor register (enable device) */
      /* SCI2C3: ORIE=1,NEIE=1,FEIE=1,PEIE=1 */
  SCI2C3 |= 0x0FU;                     /* Enable error interrupts */
  SCI2C2 |= (SCI2C2_TE_MASK | SCI2C2_RE_MASK | SCI2C2_RIE_MASK); /*  Enable transmitter, Enable receiver, Enable receiver interrupt */
}



/* END AS2. */


/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
