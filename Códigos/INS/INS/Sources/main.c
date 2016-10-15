/* ###################################################################
**     Filename    : main.c
**     Project     : I2C_ADXL345
**     Processor   : MCF51QE64CLH
**     Version     : Driver 01.00
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2016-05-11, 21:53, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "CI2C1.h"
#include "CI2C2.h"
#include "Bit1.h"
#include "AS1.h"
#include "TI1.h"
#include "AS2.h"
#include "Bit2.h"
#include "Bit3.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Functions.h"
#include "EmbeddedKalman.h"
#include "INS_GPS.h"

#define DATA_SIZE 18
/* User includes (#include below this line is not maintained by Processor Expert) */
byte DataBuffer[DATA_SIZE+1];
sensor MPU9250;
extern float Qy[2][2];
extern float Ry[2][2];
extern float Qp[2][2];
extern float Rp[2][2];
extern float Qr[2][2];
extern float Rr[2][2];

void main(void){
  /* Write your local variable definition here */
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/  
	PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
	INIT_NMEA(); 
	DELAY(1000);
	SENSOR_INIT(MPU9250,MPU9250_SLAVE_ADDRESS,BIT16,DATA_SIZE);
	MPU9250_INIT();
	ARRAY_INIT_8(DataBuffer,DATA_SIZE+1,'(');
	ADJUST_NOISE_MATRIX(Ry,Qy,50,50);
	ADJUST_NOISE_MATRIX(Rp,Qp,500,500);
	ADJUST_NOISE_MATRIX(Rr,Qr,500,500);
	
	for(;;){
		BUSY_LED(1);
		MCU_FIRMWARE();
	}
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
