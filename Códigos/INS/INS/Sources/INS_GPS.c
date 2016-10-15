/*
 * INS_GPS.c
 * Descripción: El siguiente archivo contiene las variables y funciones 
 * utilizadas para:
 * 		- Configuración del módulo GPS (Adafruit ultimate GPS)
 * 		- Adquisición e interpretación de datos del GPS (protocolo NMEA)	
 *       
 * GID Mecatrónica. Universidad Simón Bolívar. Venezuela. 
 *  Creado en: Aug 15, 2016
 *      Autores: Bruno Barone y Jhonny Rodríguez.
 */
#include "Cpu.h"
#include "INS_GPS.h"
//************************************************************************
//*						     VARIABLES GLOBALES                          *
//************************************************************************

//UPDATE RATE
byte PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ[] = "$PMTK220,10000*2F"; // Cada 10 seg, 100 millihertz.
byte PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ[] = "$PMTK220,5000*1B";  // Cada 5 seg, 200 millihertz.
byte PMTK_SET_NMEA_UPDATE_1HZ[] =  "$PMTK220,1000*1F";			  
byte PMTK_SET_NMEA_UPDATE_5HZ[] =  "$PMTK220,200*2C";			    
byte PMTK_SET_NMEA_UPDATE_10HZ[] = "$PMTK220,100*2F"; 			  


//BAUD RATE
byte PMTK_SET_BAUD_57600[] = "$PMTK251,57600*2C";
byte PMTK_SET_BAUD_9600[] = "$PMTK251,9600*17";

//SENTENCE
// turn on only the second sentence (GPRMC)
byte PMTK_SET_NMEA_OUTPUT_RMCONLY[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29";
// turn on GPRMC and GGA
byte PMTK_SET_NMEA_OUTPUT_RMCGGA[] = "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\n";
// turn on ALL THE DATA
byte PMTK_SET_NMEA_OUTPUT_ALLDATA[] = "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28";
// turn off output
byte PMTK_SET_NMEA_OUTPUT_OFF[] = "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28";

volatile boolean recvdflag;
volatile byte auxSentence1[MAXLINELENGTH];
volatile byte auxSentence2[MAXLINELENGTH];
volatile byte *currentSentence;
volatile byte *lastSentence;
byte dataGPS[12]={0,0,0,0,0,0,0,0,0,0,0,0};

short index;
extern INS_GPS GPS;

//************************************************************************
//*    			 	           FUNCIONES                                 *
//************************************************************************

/*
** ===================================================================
**     Función    :  INIT_NMEA
**
**     Descripción :
**     Esta función se encarga de inicializar las rutinas del GPS:
**     velocidad de actualización, elección de las frases y 
**     velocidad de transmision serial de las mismas. Además, 
**     inicializa ciertas variables como: coordenadas,indices, 
**     arreglos, banderas etc.
**         
**     Parámetros  : Ninguno.
**     Retorna     : Nada.
** ===================================================================
*/
void INIT_NMEA(void){
	
	word snd=0;
	AS2_SendBlock(PMTK_SET_NMEA_UPDATE_1HZ,(word)strlen((const char *)PMTK_SET_NMEA_UPDATE_1HZ),&snd);
	AS2_SendBlock(PMTK_SET_NMEA_OUTPUT_RMCGGA,(word)strlen((const char *)PMTK_SET_NMEA_OUTPUT_RMCGGA),&snd);
	//AS2_SendBlock(PMTK_SET_BAUD_9600,(word)strlen((const char *)PMTK_SET_BAUD_9600),&snd); 
	//AS2_SendBlock(PMTK_SET_BAUD_57600,(word)strlen((const char *)PMTK_SET_BAUD_57600),&snd); 
	
	recvdflag   = false;
	index = 0;
	currentSentence = auxSentence1;
	lastSentence = auxSentence2;
	GPS.latitude = GPS.longitude = GPS.altitude = 0;
	GPS.lat = GPS.lon = 0;
	GPS.latitudeDegrees = GPS.longitudeDegrees = 0;
	GPS.character = 0;
	INIT_ARRAY(GPS.latitud,9);
	INIT_ARRAY(GPS.longitud,10);
	INIT_ARRAY(GPS.altitud,5);

}

/*
** ===================================================================
**     Función    :  INIT_ARRAY
**
**     Descripción :
**     Inicializa un arreglo con ceros. Utilizado para inicializar los
**     arreglos donde se guardarán las coordenadas. 
**     
**     Parámetros  : 
**     *array		- Apuntador al arreglo a inicializar.
**     size			- Tamaño del arrego.
**     
**     Retorna     : Nada.
** ===================================================================
*/
void INIT_ARRAY(char *array,byte size){
	byte i;
		
	 for(i=0;i<size;i++){
		 array[i]=0;
	 }
	
}

/*
** ===================================================================
**     Función    :  READ_NMEA
**
**     Descripción :
**     Rutina principal encargada de la recepción de los carácteres de
**     las frases. La frase se considera completa cuando se recibe el 
**     carácter final de salto de línea 10.
**     
**     Parámetros  : Ninguno.
**     Retorna     : carácter.
** ===================================================================
*/
byte READ_NMEA(void){
	byte character = 0;
	if(AS2_GetCharsInRxBuf() <= 0) {return character;}
	(void)AS2_RecvChar(&character);
	if(character==10){
		currentSentence[index]=0;
		if (currentSentence == auxSentence1) {
			currentSentence = auxSentence2;
			lastSentence = auxSentence1;
		} else {
			currentSentence = auxSentence1;
			lastSentence = auxSentence2;
		}
		recvdflag   = true;
		index = 0;
		
	}
	currentSentence[index]=character;
	index++;
	
	if(index >= MAXLINELENGTH) index = MAXLINELENGTH - 1;

	return character;	
}

/*
** ===================================================================
**     Función     :  SENTENCE_NMEA
**
**     Descripción :
**     Se coloca la bandera de recepcion en falso dado que la frase 
**     esta completa y se devuelve el apuntador al arreglo donde
**     se encuentra la frase.
**     
**     Parámetros  : Ninguno.
**     Retorna     : Apuntador al arreglo donde se guarda la frase. 
** ===================================================================
*/
char *SENTENCE_NMEA(void){
	recvdflag   = false;
	return (char *)lastSentence;
}

/*
** ===================================================================
**     Función     :  PARSE_HEX
**
**     Descripción :
**     Función que interpreta el equivalente en decimal.
**     Parámetros  : 
**     c			- Carácter de entrada.
**     
**     Retorna     : Equivalente decimal del carácter.
** ===================================================================
*/
byte PARSE_HEX(byte c) {
    if (c < '0')
      return 0;
    if (c <= '9')
      return c - '0';
    if (c < 'A')
       return 0;
    if (c <= 'F')
       return (c - 'A')+10;
    // if (c > 'F')
    return 0;
}

/*
** ===================================================================
**     Función     :  PARSE_NMEA
**
**     Descripción :
**     Rutina encargada de la extración de los parámetros de
** 	   ínteres: latitud, longitud y altitud. Dichas variables
** 	   se encuentran almacenadas en arreglos de carácteres.
** 	   
**     Parámetros  : 
**     *array		- Apuntador al arreglo de entrada.
**     
**     Retorna     : Verdadero o falso. De retornar verdadero, la
**	             	 extracción ha sido exitosa.
** ===================================================================
*/
boolean PARSE_NMEA(volatile byte *array){
	char degreebuff[10]={0,0,0,0,0,0,0,0,0,0};
	int i;
	
	if (array[strlen((const char *)array)-4] == '*') {
		uint16_t sum = (uint16_t)(PARSE_HEX(array[strlen((const char *)array)-3]) * 16);
		sum += PARSE_HEX(array[strlen((const char *)array)-2]);
		// check checksum  
		for (i=2; i < (strlen((const char *)array)-4); i++) {
			sum ^= array[i];
		}
		if (sum != 0) {
			// bad checksum :(
			return false;
		}
		i=0;
	}
	
	if (strstr((const char *)array, "$GPGGA")) {
		 char *p = (char *) array;
		 p = strchr(p, ',')+1;
		 p = strchr(p, ',')+1;
		 
		 for(i=0;i<9;i++){
			 GPS.latitud[i]=p[i];
		 }
		 
		 p = strchr(p, ',')+1;
		 if (',' != *p){
			 if (p[0] == 'S') GPS.lat = 'S';
			 else if (p[0] == 'N') GPS.lat = 'N';
			 else if (p[0] == ',') GPS.lat = 0;
			 else return false;
		 }
		    
		 p = strchr(p, ',')+1;
		 
		 for(i=0;i<10;i++){
			 GPS.longitud[i]=p[i];
		 }
		 p = strchr(p, ',')+1;
		 if (',' != *p){
			 if (p[0] == 'W') GPS.lon = 'W';
			 else if (p[0] == 'E') GPS.lon = 'E';
			 else if (p[0] == ',') GPS.lon = 0;
			 else return false;
		 }
		    
		 p = strchr(p, ',')+1;		 
		 p = strchr(p, ',')+1;
		 p = strchr(p, ',')+1;
		 p = strchr(p, ',')+1;
		 
		 for(i=0;i<5;i++){
			 GPS.altitud[i]=p[i];
		 }
		 
		 return true;
		
	}
	
}

/*
** ===================================================================
**     Función     :  PARSE_LATITUD
**
**     Descripción :
**     Rutina encargada de la conversión del arreglo de carácteres
** 	   a número flotante. Se realiza la transformación de las
** 	   coordenadas a formato decimal.
**    
**     Parámetros  : 
**     latitud[]	- Arreglo de caracteres con las coordenadas de 	
**     				  latitud.
**     
**     Retorna     : Nada.
** ===================================================================
*/
void PARSE_LATITUD(char latitud[9]){
	char degreebuff[10]={0,0,0,0,0,0,0,0,0,0};
	int32_t degree,minutes;
	strncpy(degreebuff, latitud, 2);
	degreebuff[2] = '\0';
	degree = atol(degreebuff)* 10000000;
	latitud+=2;
	strncpy(degreebuff, latitud, 2); // minutes
	latitud += 3; // skip decimal point
	strncpy(degreebuff + 2, latitud, 4);
	degreebuff[6] = '\0';
	minutes = 50 * atol(degreebuff) / 3;
	//latitude_fixed = degree + minutes;
	GPS.latitude = degree / 100000 + minutes * 0.000006F;
	GPS.latitudeDegrees = (float)((GPS.latitude-100*(int)(GPS.latitude/100))/60.0);
	GPS.latitudeDegrees += (int)(GPS.latitude/100);
	if(GPS.lat=='S'){
		GPS.latitudeDegrees *= -1.0;
	}
	
	
}

/*
** ===================================================================
**     Función     :  PARSE_LONGITUD
**
**     Descripción :
**     Rutina encargada de la conversión del arreglo de carácteres
** 	   a número flotante. Se realiza la transformación de las
** 	   coordenadas a formato decimal.
** 	   
**     Parámetros  : 
**     longitud[]	- Arreglo de caracteres con las coordenadas de 	
**     				  longitud.
**     
**     Retorna     : Nada.
** ===================================================================
*/
void PARSE_LONGITUD(char longitud[10]){
	char degreebuff[10]={0,0,0,0,0,0,0,0,0,0};
	int32_t degree,minutes;
	// parse out longitude		        
	strncpy(degreebuff, longitud, 3);
	longitud += 3;
	degreebuff[3] = '\0';
	degree = atol(degreebuff) * 10000000;
	strncpy(degreebuff, longitud, 2); // minutes
	longitud += 3; // skip decimal point
	strncpy(degreebuff + 2, longitud, 4);
	degreebuff[6] = '\0';
	minutes = 50 * atol(degreebuff) / 3;
	GPS.longitude = degree / 100000 + minutes * 0.000006F;
	GPS.longitudeDegrees = (float)((GPS.longitude-100*(int)(GPS.longitude/100))/60.0);
	GPS.longitudeDegrees += (int)(GPS.longitude/100);
	if(GPS.lon=='W'){
		GPS.longitudeDegrees *= -1.0;
	}
}

/*
** ===================================================================
**     Función     :  PARSE_ALTITUD
**
**     Descripción :
**     Rutina encargada de la conversión del arreglo de carácteres a 
** 	   número flotante. Se realiza la transformación de las coordenadas
** 	   coordenadas a formato decimal.
** 	   
**     Parámetros  : 
**     altitud[]	- Arreglo de caracteres con las coordenadas de 	
**     				  altitud.
**     				  
**     Retorna     : Nada.
** ===================================================================
*/	
void PARSE_ALTITUD(char altitud[10]){
	
	GPS.altitude = (float)(atof(altitud));
}

/*
** ===================================================================
**     Función     :  FLOAT_TO_BYTE
**
**     Descripción :
**     El número flotante se divide en cuatro bytes individuales
** 	   para su envío serial.
**     
**     Parámetros  : 
**     number 		- Coordenada en punto flotante.
**     *dataGps		- Apuntador al arreglo almacenará su división.
**     
**     Retorna     : Nada.
** ===================================================================
*/
void FLOAT_TO_BYTE(float number,byte *dataGPS){
    byte* ptr= (byte *)&number;
    dataGPS[0]=*ptr;
    dataGPS[1]=*(ptr+1);
    dataGPS[2]=*(ptr+2);
    dataGPS[3]=*(ptr+3);
}
