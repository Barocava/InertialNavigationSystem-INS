/*
 * INS_GPS.h
 * Descripci�n: El siguiente archivo contiene las variables y funciones 
 * utilizadas para:
 * 		- Configuraci�n del m�dulo GPS (Adafruit ultimate GPS)
 * 		- Adquisici�n e interpretaci�n de datos del GPS (protocolo NMEA)	
 *       
 * GID Mecatr�nica. Universidad Sim�n Bol�var. Venezuela. 
 *  Creado en: Aug 15, 2016
 *      Autores: Bruno Barone y Jhonny Rodr�guez.
 */

#ifndef INS_GPS_H_
#define INS_GPS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
#include "AS1.h"
#include "AS2.h"


//************************************************************************
//*                             DEFINICIONES                             *
//************************************************************************

#define MAXLINELENGTH 120

//************************************************************************
//*                     DEFINICI�N DE TIPOS DE DATOS                     *
//************************************************************************

typedef struct INS_GPS {
	
	float latitude, longitude, altitude;
	float latitudeDegrees, longitudeDegrees;
    char lat, lon;
	byte character;
	char latitud[9];
	char longitud[10];
	char altitud[5];
	
} INS_GPS;

typedef enum {false,true} boolean;

//************************************************************************
//*    			 	           FUNCIONES                                 *
//************************************************************************

void INIT_NMEA(void);
void INIT_ARRAY(char *array,byte size);
byte READ_NMEA(void);
char *SENTENCE_NMEA(void);
boolean PARSE_NMEA(volatile byte *array);
byte PARSE_HEX(byte c);
void PARSE_LATITUD(char latitud[9]);
void PARSE_LONGITUD(char longitud[10]);
void PARSE_ALTITUD(char altitud[5]);
void FLOAT_TO_BYTE(float number,byte *dataGPS);

#endif /* INS_GPS_H_ */
