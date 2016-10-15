/*
 * Functions.h
 * Descripción: El siguiente archivo contiene la rutina principal del INS así como las variables y funciones 
 * utilizadas principalmente para:
 *		 - El cómputo de funciones trigonométricas.
 *		 - Operaciones de transacción de datos de I2C.
 *		 - Envío de datos vía UART.
 *		 - Misceláneos.
 * 
 * GID Mecatrónica. Universidad Simón Bolívar. Venezuela. 
 * Creado en: Mayo 15, 2016
 *      Autores: Bruno Barone y Jhonny Rodríguez
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "CI2C1.h"
#include "Bit1.h"
#include "AS1.h"
#include "Bit2.h"
#include "Bit3.h"
#include "EmbeddedKalman.h"
#include "INS_GPS.h"

//************************************************************************
//*     DIRECCIONES DE LOS REGISTROS DE LOS DISPOSITIVOS ESCLAVOS        *
//************************************************************************

//MPU9250
#define MPU9250_WHO_AM_I            0x75 
#define MPU9250_SLAVE_ADDRESS		0x68
#define MPU9250_ACCEL_XOUT_H        0x3B   
#define MPU9250_ACCEL_XOUT_L        0x3C  
#define MPU9250_ACCEL_YOUT_H        0x3D   
#define MPU9250_ACCEL_YOUT_L        0x3E  
#define MPU9250_ACCEL_ZOUT_H        0x3F     
#define MPU9250_ACCEL_ZOUT_L        0x40     
#define MPU9250_TEMP_OUT_H          0x41     
#define MPU9250_TEMP_OUT_L          0x42     
#define MPU9250_GYRO_XOUT_H         0x43     
#define MPU9250_GYRO_XOUT_L         0x44     
#define MPU9250_GYRO_YOUT_H         0x45     
#define MPU9250_GYRO_YOUT_L         0x46     
#define MPU9250_GYRO_ZOUT_H         0x47     
#define MPU9250_GYRO_ZOUT_L         0x48
#define MPU9250_PWR_MGMT_1          0x6B
#define MPU9250_GYRO_CONFIG			0x1B
#define MPU9250_ACCEL_CONFIG        0x1C  
#define MPU9250_SMPRT_DIV			0x19
#define MPU9250_CONFIG 				0x1A
#define MPU9250_INT_PIN_CFG	    	0x37


//AK8963 - MPU9250 magnetometro
#define AK8963_SLAVE_ADDRESS	    0x0C
#define AK8963_WHO_AM_I				0x00
#define AK8963_WHO_AM_I_VALUE		0x48
#define AK8963_CNTL1				0x0A
#define AK8963_CNTL2				0x0B
#define AK8963_HXL					0x03
#define AK8963_ST1					0x02

#define PI 							3.1416	
#define ERROR_DIV                    2
#define SUCCESS                      1

//************************************************************************
//*                     DEFINICIÓN DE TIPOS DE DATOS                     *
//************************************************************************


typedef enum DeviceResolution {BIT16,BIT14,BIT12,BIT10,BIT8} res; 

typedef struct SENSOR {
	byte deviceAddress;			// Dirección del dispositivo (I2C)
	byte dataRaw[20];			// Data cruda proveniente del dispositivo (8 bits)
	byte dataRawSize;		    // Tamaño del buffer de la data "cruda" (8 bits)
	short dataFrame[10];		// Data unificada en elementos de 16 bits
	byte dataFrameSize;		    // Tamaño del buffer de la data de 16 bits
	res dataRes;				// Resolución de la data del dispositivo
	
} sensor;


//************************************************************************
//*    			 	   PROTOTIPO DE LAS FUNCIONES                        *
//************************************************************************

//FUNCIONES TRIGONOMÉTRICAS
float ATAN(float y);
float ATAN2(float num,float den);
float ASIN(float y);
float COS(float angle);
void DEG_TO_RAD(float *x,byte size);
void RAD_TO_DEG(float *x,byte size);

//INICIALIZACIÓN Y MANEJO DE ARREGLOS
void ARRAY_INIT_8(byte *Array,byte size,byte header);
void ARRAY_INIT_16(short *Array,byte size,byte header);
void DATA_MERGE(byte *InBuffer, short *OutBuffer,byte InBufferSize);

// INICIALIZACIÓN DE LOS SENSORES
void SENSOR_INIT(sensor mySensor,byte slaveAddress,res sensorRes, byte RawSize);
void MPU9250_INIT(void);
void AK8963_INIT(void);

//FUNCIONES DE I2C
byte I2C_READ(byte addressReg);
void I2C_MULTIPLE_READ(byte InitialAddress,byte *DataArray,word size);
byte I2C_WRITE_REGISTER(byte addressReg,byte data);
void I2C_MULTIPLE_WRITE(byte *data,byte size);
void I2C_SELECT_SLAVE(byte slaveAddress);
void MPU9250_READ(void);
void AK8963_READ(byte *dataFrame);

//FUNCIONES PARA UART
void RESOLVE_HEADER_CONFLICT(byte header,word size);
void SEND_FRAME(AS1_TComData *dataBlock,word size);
void DATA_SPLIT(short *InBuffer, byte *OutBuffer,byte InBufferSize);
void CONVERT_DATA(float *dataIn,short *dataOut,byte size);
void PUSH_TO_BUFFER(byte *InBuffer, byte *OutBuffer,byte InBufferSize);

//FUNCIONES DEL MCU Y MISCELANEOS
void DELAY(word ms);
void SET_FLAG(byte* flag);
void CLR_FLAG(byte* flag);
void BUSY_LED(byte state);
void TRANSLATE_FRAME(void);
void MCU_FIRMWARE(void);


#endif /* FUNCTIONS_H_ */
