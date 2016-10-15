/*
 * Functions.c
 * Descripci�n: El siguiente archivo contiene la rutina principal del INS as� como las variables y funciones 
 * utilizadas principalmente para:
 *		 - El c�mputo de funciones trigonom�tricas.
 *		 - Operaciones de transacci�n de datos de I2C.
 *		 - Env�o de datos v�a UART.
 *		 - Miscel�neos.
 * 
 * GID Mecatr�nica. Universidad Sim�n Bol�var. Venezuela. 
 * Creado en: Mayo 15, 2016
 *      Autores: Bruno Barone y Jhonny Rodr�guez
 */

#include "Cpu.h"
#include "Functions.h"


//************************************************************************
//*						     VARIABLES GLOBALES                          *
//************************************************************************

byte sendFlag=1;                      				     // Bandera utilizada para indicar si se est� listo para mandar los datos por serial 
int contador=0;                     				     // contadores para los indicadores LEDs
int timeCounter=0;
short aux[10]={0,0,0,0,0,0,0,0,0,0};
byte errorCode=SUCCESS;
extern sensor MPU9250;
extern byte DataBuffer[19];
extern float Qr[2][2];
extern float pplus[2][2];
extern float Rr[2][2];
extern float Qp[2][2];
extern float Rp[2][2];
extern float Qy[2][2];
extern float Ry[2][2];
extern float Pr[2][2];
extern float Pp[2][2];
extern float Py[2][2];
extern float xr[2];
extern float xp[2];
extern float xy[2];
extern float accel[3];
extern float gyro[3];
extern float mag[3];
extern float RPY[3];
extern float ER[3];

INS_GPS GPS;
extern volatile boolean recvdflag;
extern volatile byte *lastSentence;
extern byte dataGPS[12];

//************************************************************************
//*						FUNCIONES Y PROCEDIMIENTOS                       *
//************************************************************************

// *************** TRIGONOMETR�A ****************


/*
** ===================================================================
**     Funci�n     :  ATAN
**
**     Descripci�n :
**     Computa la funci�n arcotangente. Rango: [-PI/2,PI/2]. 
**       
**     Par�metros  : 
**     
**     y     - Argumento de la funci�n ATAN. Dominio: R.
** 
**     Retorna     : angle - Resultado de la funci�n en radianes.
** ===================================================================
*/

float ATAN(float y){                               
	float x=y;
	float angle=0;
    if(x>1.4 || x<-1.4){
        angle= ATAN(1/x);
        if(x>0){
            angle= (float)(PI/2 - angle);
        }
        else{
            angle= (float)(-PI/2 -angle);
        }
    }
    else{
        if (x>=0.5){
            angle= (float)(0.015 + x*(1.0311+ x*(-0.2608)));     // L�nea de tendencia de Excel
        }
        else if (x<=-0.5){
        	angle= (float)(-0.015 + x*(1.0311+ x*(0.2608)));     // L�nea de tendencia de Excel
        }
        else{
        angle=x*x;
        angle= (float)(x*(1.0+angle*(-0.333+angle*(0.2+angle*(-0.1429)))));
        }
    }
    return angle;
}

/*
** ===================================================================
**     Funci�n     :  ATAN2
**
**     Descripci�n :
**     Computa la funci�n arcotangente 2. Rango: [-PI,PI]. 
**         
**     Par�metros  :
**      num - Numerador de la fracci�n.
**      den - Denominador de la fracci�n.
**            Dominio: R.
** 
**     Retorna     : angle - Resultado de la funci�n en radianes.    				 
** ===================================================================
*/

float ATAN2(float  num,float den){                                   
	double angle=0;
    if(den==0){
    	if(num>0){
    		return PI/2;
    	}
    	else if(num<0){
    		return -PI/2;
    	}
    	else{
    		return 0;
    	}
    }
    else if(num>0.0 && den<0.0){
			angle= ATAN(num/den)+PI;
		}
	else if(num<0.0 && den<0.0){
			angle= ATAN(num/den)-PI;
		}
	else{
			angle=ATAN(num/den);
		}
	return (float)(angle);
}

/*
** ===================================================================
**     Funci�n     :  ASIN
**
**     Descripci�n :
**     Computa la funci�n arcoseno. Rango: [-PI/2,PI/2]. 
**     Error del 6% en valores extremos. 
**         
**     Par�metros  : 
**     
**     y     - Argumento de la funci�n. Dominio [-1,1].
** 
**     Retorna     : angle - Resultado de la funci�n en radianes.     				 
** ===================================================================
*/

float ASIN(float y){						 				
	float x=y;
    double angle=0;
	if(x<-1.0){
		x=-1.0;
	}
	else if(x>1.0){
		x=1.0;
	}
    if (x>=0.75){
    	angle= - 21.489 + x*(78.787 +x*(-93.967 + x*38.142));    // L�nea de tendencia de Excel
	}
	else if (x<=-0.75){	    
	    angle= + 21.489 + x*(78.787 +x*(+93.967 + x*38.142));    // L�nea de tendencia de Excel
	}
	else{
	    angle=x*x;
	    angle= x*(1+angle*(0.1667+angle*(0.075+angle*(0.0446+angle*(0.039)))));
	}
	return (float)(angle);
}

/*
** ===================================================================
**     Funci�n     : COS
**     Descripci�n :
**     
**     Computa la funci�n coseno. Rango[0,1].
**         
**     Par�metros  :
**      
**     y   - Argumento de la funci�n en radianes. Dominio [-pi/2,pi/2].
** 
**     Retorna     : result - Resultado de la funci�n coseno.      				 
** ===================================================================
*/
float COS(float angle){										  
	float x=angle;
    double result=x*x;
    result = 1+result*(-0.5+result*(0.0417+result*(-0.0014)));
    return (float)result;
}

/*
** ===================================================================
**     Funci�n     : RAD_TO_DEG
**     Descripci�n :
**     
**     Funci�n para convertir radianes a grados.
**         
**     Par�metros  : 
**     
**     *x    - apuntador al arreglo de n�meros a pasar de radianes a 
**             grados.
** 
**     Retorna     : Nada.    				 
** ===================================================================
*/

void RAD_TO_DEG(float *x,byte size){					   
	byte i;
	for(i=0;i<size;i++){
		x[i]= (float)(x[i]*180/PI);
	}
}

/*
** ===================================================================
**     Funci�n     : DEG_TO_RAD
**     Descripci�n :
**     
**     Funci�n para convertir grados a radianes.
**         
**     Par�metros  :
**     
**     *x    - apuntador al arreglo de n�meros a pasar 
**             de radianes a grados.
**
**     Retorna     : Nada.      				 
** ===================================================================
*/

void DEG_TO_RAD(float *x,byte size){
	
	byte i;
	for(i=0;i<size;i++){
		x[i]= (float)(x[i]*PI/180);
	}
}

// *************** ARREGLOS DE DATOS ****************

/*
** ===================================================================
**     Funci�n     : ARRAY_INIT_8
**     Descripci�n : 
**     
**     Funci�n para inicializar un arreglo de elementos de 1 Byte.
**         		            
**     Par�metros  : 
**     
**     *Array     - Apuntador al arreglo a inicializar.
**     size       - Tama�o del arreglo.
**     header     - Valor de la cabecera que sea insertar. 
**     				(Dejar en cero si no se desea cabecera)
**
**     Retorna     : Nada.      				 
** ===================================================================
*/

void ARRAY_INIT_8(byte *Array,byte size,byte header){ 
	
	byte i=0;
	Array[0]= header;
	for(i=1;i<size;i++){
		Array[i]=0;
	}
}

/*
** ===================================================================
**     Funci�n     : ARRAY_INIT_16
**     Descripci�n : 
**     
**     Funci�n para inicializar un arreglo de elementos de 2 Bytes.
**         		            
**     Par�metros  : 
**     
**     *Array     - Apuntador al arreglo a inicializar.
**     size       - Tama�o del arreglo.
**     header     - Valor de la cabecera que sea insertar. 
**     				(Dejar en cero si no se desea cabecera)
**
**     Retorna     : Nada.      				 
** ===================================================================
*/

void ARRAY_INIT_16(short *Array,byte size,byte header){
	// 
	byte i=0;
	Array[0]= header;
	for(i=1;i<size;i++){
		Array[i]=0;
	}
}

/*
** ===================================================================
**     Funci�n     : DATA_MERGE
**     Descripci�n : 
**     Rutina para juntar paquetes de 8 bits en paquetes de 16 bits.
**	   InBufferSize debe ser PAR de lo contrario habr� perdida de datos.
**         		            
**     Par�metros  :
**     
**     *InBuffer     - Apuntador al arreglo de datos de 1 byte.
**     *OutBuffer	 - Apuntador al arreglo de datos de 2 bytes.
**     InBufferSize	 - Tama�o del arreglo de entrada.
**
**     Retorna     : Nada.      				 
** ===================================================================
*/

void DATA_MERGE(byte *InBuffer, short *OutBuffer,byte InBufferSize){
	byte i;
	for(i=0;i<InBufferSize/2;i++){
		OutBuffer[i]=  InBuffer[2*i];     // x1
		OutBuffer[i]<<=8;   			  //
		OutBuffer[i]|= InBuffer[2*i+1];   // x= x0+x1
	}
}

//*************** I2C GEN�RICO ****************

/*
** ===================================================================
**     Funci�n     : I2C_WRITE_REGISTER
**     Descripci�n :
**     Rutina para mandar dos datos: la direcci�n de registro y el 
**     contenido del mismo.(Destino: Dispostivo esclavo)    		            
**     
**     Par�metros  :
**     
**     addressReg	 - Direcci�n del registro a escribir.
**     data			 - Contenido a escribir en el registro.  
**
**     Retorna     :
**     
**     err           - C�digo de error de processor expert       				 
** ===================================================================
*/

byte I2C_WRITE_REGISTER(byte addressReg,byte data){
	
	word snd=0;
	word size=2;
	byte err;
	byte frame[2];
	frame[0]=addressReg;
	frame[1]=data;
	err=CI2C1_SendBlock(frame,size,&snd);
	return err; // Retornar c�digo de error definido por processor expert
}

/*
** ===================================================================
**     Funci�n     : I2C_MULTIPLE_WRITE 
**     Descripci�n :
**     Funci�n para escribir m�ltiples datos en el dispositivo esclavo.
**         		            
**     Par�metros  :
**     *data	 - Apuntador al arreglo de datos a mandar por I2C.
**     size		 - Tama�o del arreglo de datos.
**
**     Retorna     : Nada.
** ===================================================================
*/

void I2C_MULTIPLE_WRITE(byte *data,byte size){
	word snd;
	CI2C1_SendBlock(data,size,&snd);
}

/*
** ===================================================================
**     Funci�n     : I2C_READ 
**     Descripci�n : 
**     Rutina para recibir un byte del dispositivo esclavo.
**         		            
**     Par�metros  :
**     addressReg	 - Direcci�n del registro del dispositivo esclavo 
**     				   que se desea leer.
**
**     Retorna     : 
**     
**     data			 - Lectura del registro del esclavo.
** ===================================================================
*/

byte I2C_READ(byte addressReg){
	byte data =0;
	CI2C1_SendChar(addressReg);
	CI2C1_RecvChar(&data);
	return data;
}
  
/*
** ===================================================================
**     Funci�n     : I2C_MULTIPLE_READ 
**     Descripci�n :
**     Rutina para recibir multiples bytes del dispositivo esclavo .  		            
**     
**     Par�metros  :
**     InitialAddress	- Direcci�n de registro inicial. A partir de 
**     					  esta direcci�n se realiza la lectura m�ltiple.
**     *DataArray		- Apuntador al arreglo de datos donde se 
**     					  almacenar�n la informaci�n adquirida de I2C.
**
**		size			- Tama�o del arreglo de datos.
**     
**     Retorna     : Nada.
**            				 
**     
** ===================================================================
*/

void I2C_MULTIPLE_READ(byte InitialAddress,byte *DataArray,word size){
	
	word snd=0;
	CI2C1_SendChar(InitialAddress);
	CI2C1_RecvBlock(DataArray,size,&snd);
}

/*
** ===================================================================
**     Funci�n     : I2C_SELECT_SLAVE
**     Descripci�n :
**     Rutina para seleccionar satisfactoriamente un nuevo esclavo I2C.
**     
**         		            
**     Par�metros  :
**     slaveAddress    - Direcci�n del esclavo que se desea seleccionar.
**
**     Retorna     : Nada.    				 
** ===================================================================
*/

void I2C_SELECT_SLAVE(byte slaveAddress){
	while(CI2C1_SelectSlave(slaveAddress)!=ERR_OK) ;
}

//*************** DATA SENSORES ****************

/*
** ===================================================================
**     Funci�n     : SENSOR_INIT
**     Descripci�n :
**     Funci�n para inicializar estructura de datos: "Sensor".
**         		            
**     Par�metros  :
**     mySensor		- Tipo de dato "sensor" a inicializar.
**     slaveAddress - Direcci�n de esclavo de mySensor.
**     
**     Retorna     : Nada. (Modifica variables globales)
** ===================================================================
*/

void SENSOR_INIT(sensor mySensor,byte slaveAddress,res sensorRes,byte RawSize){
	mySensor.deviceAddress=slaveAddress;
	mySensor.dataRes=sensorRes;
	mySensor.dataRawSize=RawSize;
	mySensor.dataFrameSize=RawSize/2;
	ARRAY_INIT_8(mySensor.dataRaw,mySensor.dataRawSize,0);
	ARRAY_INIT_16(mySensor.dataFrame,mySensor.dataFrameSize,0);
}

/*
** ===================================================================
**     Funci�n     : MPU9250_INIT
**     Descripci�n : 
**     Rutina para inicializar la MPU9250.
**         		            
**     Par�metros  : Ninguno.
**
**     Retorna     : Nada.      	
** ===================================================================
*/

void MPU9250_INIT(void){
	while(CI2C1_SelectSlave(MPU9250_SLAVE_ADDRESS)!=ERR_OK)
	ARRAY_INIT_8(MPU9250.dataRaw,20,0);
	ARRAY_INIT_16(MPU9250.dataFrame,10,0);
	I2C_WRITE_REGISTER(MPU9250_PWR_MGMT_1,0x00);  // Evitar el sleep mode
	DELAY(100);
	I2C_WRITE_REGISTER(MPU9250_SMPRT_DIV,0x00);   // Dejamos la tasa de muestreo en 8kHz la cual ser� truncada por el DLPF
	I2C_WRITE_REGISTER(MPU9250_CONFIG,0x00);	  // DLPF
	I2C_WRITE_REGISTER(MPU9250_GYRO_CONFIG,0x08); // +/- 500 dps, no self test
	I2C_WRITE_REGISTER(MPU9250_ACCEL_CONFIG,0x00);// +/- 2g, no self test
	I2C_WRITE_REGISTER(MPU9250_INT_PIN_CFG,0x02); //Saltar el I2C auxiliar para acceder directamente el aceler�metro interno 
	DELAY(10);                        			  // Espera necesaria para configurar el dispositivo
	AK8963_INIT();								  // Funci�n para configurar el magnet�metro interno
}

/*
** ===================================================================
**     Funci�n    : AK8963_INIT
**     Descripci�n:
**     Rutina para inicializar el magnet�metro AK8963.
**         		            
**     Par�metros  : Ninguno.
** 
**     Retorna     : Nada.      				 
** ===================================================================
*/

void AK8963_INIT(void){
	DELAY(10);                        			// Espera necesaria para configurar el dispositivo
	while(CI2C1_SelectSlave(AK8963_SLAVE_ADDRESS)!=ERR_OK) ;
	I2C_WRITE_REGISTER(AK8963_CNTL1,0x16);      // Medidas continuas a 100Hz , 16 bit de resoluci�n 
	DELAY(1);
}

/*
** ===================================================================
**     Funci�n     : MPU9250_READ
**     Descripci�n :
**     Rutina encargada de la adquisici�n de los datos de la MPU9250.
**     
**     Par�metros  : Ninguno.
**
**     Retorna     : Nada (Modifica variables globales).      				 
** ===================================================================
*/

void MPU9250_READ(void){
	byte magnetoFrame[8]={0,0,0,0,0,0,0,0}; // Son seis bytes de datos pero deben leerse obligatoriamente los dos registros de STATUS
	byte tempFrame[2];
	I2C_SELECT_SLAVE(MPU9250_SLAVE_ADDRESS);
	I2C_MULTIPLE_READ(MPU9250_ACCEL_XOUT_H,MPU9250.dataRaw,14);
	AK8963_READ(magnetoFrame);
	tempFrame[0]= MPU9250.dataRaw[6];	   // Se guarda temporalmente las lecturas de temperatura 
	tempFrame[1]= MPU9250.dataRaw[7];	   // Para reordenarlas posteriormente junto con los demas datos	
	MPU9250.dataRaw[6]=MPU9250.dataRaw[8]; // Corremos todos los datos dos posiciones hacia el inicio
	MPU9250.dataRaw[7]=MPU9250.dataRaw[9];
	MPU9250.dataRaw[8]=MPU9250.dataRaw[10];
	MPU9250.dataRaw[9]=MPU9250.dataRaw[11];
	MPU9250.dataRaw[10]=MPU9250.dataRaw[12];
	MPU9250.dataRaw[11]=MPU9250.dataRaw[13];       
	MPU9250.dataRaw[12]=magnetoFrame[2]; //
	MPU9250.dataRaw[13]=magnetoFrame[1]; //
	MPU9250.dataRaw[14]=magnetoFrame[4]; // LittleEndian (formato magnetometro) a BigEndian (formato accel. y gyro.) 
	MPU9250.dataRaw[15]=magnetoFrame[3]; // y uni�n de datos al buffer principal
	MPU9250.dataRaw[16]=magnetoFrame[6]; //
	MPU9250.dataRaw[17]=magnetoFrame[5]; //
	MPU9250.dataRaw[18]=tempFrame[0];    // Se vuelve a colocar los datos de temperatura, pero ahora al final de la trama
	MPU9250.dataRaw[19]=tempFrame[1];    //
	//TRAMA FINAL: |ACCEL||GYRO||MAG||TEMP|
	//                1      2    3     4
}

/*
** ===================================================================
**     Funci�n     : AK8963_READ 
**     Descripci�n :
**     Funci�n para leer del magnet�metro interno del MPU9250.
**         		            
**     Par�metros  : 
**     *dataFrame	- Apuntador a la variable donde se guardar�n las 
**     				  lecturas
**
**     Retorna     : Nada.        				 
** ===================================================================
*/

void AK8963_READ(byte *dataFrame){
	I2C_SELECT_SLAVE(AK8963_SLAVE_ADDRESS);
	I2C_MULTIPLE_READ(AK8963_ST1,dataFrame,8);
}

// *************** UART ****************

/*
** ===================================================================
**     Funci�n     : RESOLVE_HEADER_CONFLICT
**     Descripci�n :
**     Rutina para evitar que algun dato del buffer de env�o tenga el
**     mismo valor que el header de la trama. 
**                 
**     Par�metros  :
**     header		- Valor del header de la trama.
**     size			- Tama�o del buffer de env�o.
**
**     Retorna     : Nada.
** ===================================================================
*/

void RESOLVE_HEADER_CONFLICT(byte header,word size){ 
	
	byte i;
	for(i=1;i<size;i++){
		if(DataBuffer[i]==header){
			DataBuffer[i]=DataBuffer[i]-1;
		}
	}
}

/*
** ===================================================================
**     Funci�n     : DATA_SPLIT
**     Descripci�n :
**     Procedimiento para separar el paquete datos de dos bytes en dos
**     paquetes de un byte. La primera posici�n de OutputBuffer esta
**     reservada a la cabecera de la trama. Orden: LittleEndian.    		            
**     
**     Par�metros  :
**     *InBuffer     - Apuntador al arreglo de datos de 2 bytes.
**     *OutBuffer	 - Apuntador al arreglo de datos de 1 byte.
**     InBufferSize	 - Tama�o del arreglo de entrada.
**
**     Retorna     : Nada.		 
** ===================================================================
*/

void DATA_SPLIT(short *InBuffer, byte *OutBuffer,byte InBufferSize){
	byte i;
	for(i=0;i<InBufferSize;i++){
		OutBuffer[2*i+1]= (byte)(InBuffer[i]&0x00FF);      // LSB
		OutBuffer[2*i+2]= (byte)((InBuffer[i]&0xFF00)>>8); // MSB
	}
}

/*
** ===================================================================
**     Funci�n     : SEND_FRAME
**     Descripci�n :
**     Funci�n para enviar paquetes de 8 bits por serial
**         		            
**     Par�metros  :
**     *dataBlock	- Apuntador al arreglo de datos a enviar.
**     size			- Tama�o de la trama de env�o.
**
**     Retorna     : Nada.       				 
** ===================================================================
*/

void SEND_FRAME(AS1_TComData *dataBlock,word size){ 
	word snd=0;
	RESOLVE_HEADER_CONFLICT('(',size);
	AS1_SendBlock(dataBlock,size,&snd);
}

/*
** ===================================================================
**     Funci�n    : CONVERT_DATA
**     Descripci�n:
**     Funci�n para convertir datos de un arreglo de float a short. 
	   Se multiplica por 10 para conservar un decimal de precisi�n.
**         		            
**     Par�metros  :
**     *dataIn		- Apuntador al arreglo de datos tipo float. 
**     *dataOut		- Apuntador al arreglo de datos tipo short.
**     size			- Tama�o del buffer de datos de entrada.
**
**     Retorna     : Nada.		 
** ===================================================================
*/

void CONVERT_DATA(float *dataIn,short *dataOut,byte size){
	byte i;
	for(i=0;i<size;i++){
		dataOut[i]=(short)(dataIn[i]*10);
	}
}

/*
** ===================================================================
**     Funci�n     : PUSH_TO_BUFFER
**     Descripci�n :
**     Funci�n utilizada para pasar datos de un arreglo a otro.
**         		            
**     Par�metros  :
**     *InBuffer     - Apuntador al arreglo de datos de entrada
**     *OutBuffer	 - Apuntador al arreglo de datos de salida
**     InBufferSize	 - Tama�o del arreglo de entrada.
**
**     Retorna     : Nada.			 
** ===================================================================
*/

void PUSH_TO_BUFFER(byte *InBuffer, byte *OutBuffer,byte InBufferSize){
	byte i;
	for(i=0;i<InBufferSize;i++){
		OutBuffer[i]= InBuffer[i];
	}
}

// *************** GENERALES MCU ****************

/*
** ===================================================================
**     Funci�n     : TRANSLATE_FRAME 
**     Descripci�n :
**     Funci�n para cambiar el sistema de referencia del MPU9250 y 
**     hacerlo coincidir con el de la plataforma m�vil. Sistema : NED.
**	            		            
**     Par�metros  : Ninguno.
**
**     Retorna     : Nada (Modifica variables globales).       				 
** ===================================================================
*/

void TRANSLATE_FRAME(void){
	short aux=0;
	aux=MPU9250.dataFrame[0];
	MPU9250.dataFrame[0]=MPU9250.dataFrame[1]; // ax = ay
	MPU9250.dataFrame[1]=aux;                   // ay = ax
	aux=MPU9250.dataFrame[3];
	MPU9250.dataFrame[3]=-MPU9250.dataFrame[4]; // wx = -wy
	MPU9250.dataFrame[4]=-aux; 				    // wy = -wx
	MPU9250.dataFrame[5]=-MPU9250.dataFrame[5]; // wz = -wz
	MPU9250.dataFrame[6]=-MPU9250.dataFrame[6]; // mx = -mx
	MPU9250.dataFrame[7]=-MPU9250.dataFrame[7]; // my = -my
	
}

/*
** ===================================================================
**     Funci�n     : DELAY 
**     Descripci�n :
**     Procedimiento de retardo medido en milisegundos de espera.
**         		            
**     Par�metros  :
**     ms		- N�mero de milisegundos a esperar.
**
**     Retorna     : Nada.       				 
** ===================================================================
*/

void DELAY(word ms){       
	Cpu_Delay100US(10*ms);
}

/*
** ===================================================================
**     Funci�n     : SET_FLAG
**     Descripci�n :
**     Procedimiento para encender banderas.
**         		            
**     Par�metros  :
**     *flag		-Apuntador a la variable de la bandera.
**
**     Retorna     : Nada. 				 
** ===================================================================
*/

void SET_FLAG(byte *flag){ 
	*flag=1;
}

/*
** ===================================================================
**     Funci�n     : CLR_FLAG
**     Descripci�n :
**     Procedimiento para apagar las banderas.    		            
**     Par�metros  :
**     *flag		-Apuntador a la variable de la bandera.
**
**     Retorna     : Nada. 
** ===================================================================
*/

void CLR_FLAG(byte *flag){ 
	*flag=0;
}

/*
** ===================================================================
**     Funci�n     : BUSY_LED
**     Descripci�n :
**     Indicador visual de que tan ocupado est� el MCU.
**         		            
**     Par�metros  :
**     state 		- Estado en el que se colocar� el LED (on/off).
**
**     Retorna     : Nada.       				 
** ===================================================================
*/

void BUSY_LED(byte state){ 
	if(state){
		Bit3_ClrVal();
	}
	else{
		Bit3_SetVal();
	}
}

/*
** ===================================================================
**     Funci�n     : MCU_FIRMWARE 
**     Descripci�n :
**     Cuerpo principal del firmware del microcontrolador.
**                 
**     Par�metros  : Ninguno.
**     Retorna     : Nada.     				 
** ===================================================================
*/

void MCU_FIRMWARE(void){ 
	byte rawDataSize=18;
	byte sendData= 18;
	word sendSize= (word)(sendData+1);
	BUSY_LED(0);
	if(sendFlag){	  
		MPU9250_READ();
        DATA_MERGE(MPU9250.dataRaw,MPU9250.dataFrame,rawDataSize);
        TRANSLATE_FRAME();
        ACQUIRE_DATA();
        GET_RPY();
        errorCode=EULER_RATES(RPY[1]);
        RAD_TO_DEG(RPY,3);
        errorCode=KALMAN_FILTER(RPY[2],ER[2],xy,Py,Qy,Ry);
        errorCode=KALMAN_FILTER(RPY[1],ER[1],xp,Pp,Qp,Rp);
        errorCode=KALMAN_FILTER(RPY[0],ER[0],xr,Pr,Qr,Rr);
        CONVERT_DATA(xr,(aux),1);
        CONVERT_DATA(xp,(aux+1),1);
        CONVERT_DATA(xy,(aux+2),1);
        DATA_SPLIT(aux,DataBuffer,sendData/2);
		CLR_FLAG(&sendFlag);		  // Se limpia la bandera
	}
	if(errorCode==ERROR_DIV){
		Bit1_ClrVal();
	}
	else{
		Bit1_SetVal();
	}
	if(recvdflag==true){
	  if(PARSE_NMEA((volatile byte *)SENTENCE_NMEA()) == true){
		  PARSE_LATITUD(GPS.latitud);
		  PARSE_LONGITUD(GPS.longitud);
		  PARSE_ALTITUD(GPS.altitud);
		  FLOAT_TO_BYTE(GPS.latitudeDegrees,dataGPS);
		  FLOAT_TO_BYTE(GPS.longitudeDegrees,(dataGPS+4));
		  FLOAT_TO_BYTE(GPS.altitude,(dataGPS+8));
		  PUSH_TO_BUFFER(dataGPS,(DataBuffer+6),12);
	   }
	}
}
