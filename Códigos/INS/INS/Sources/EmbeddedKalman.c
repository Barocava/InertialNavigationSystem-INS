/*
 * EmbeddedKalman.c
 * Descripción: El siguiente archivo contiene las variables y funciones 
 * utilizadas para:
 * 		- Cálculo de los ángulos RPY.	
 *      - La ejecución del filtro de Kalman embebido del INS.
 * 
 * GID Mecatrónica. Universidad Simón Bolívar. Venezuela. 
 *  Creado en: Aug 7, 2016
 *      Autores: Bruno Barone y Jhonny Rodríguez.
 */

#include "EmbeddedKalman.h"

//************************************************************************
//*						     VARIABLES GLOBALES                          *
//************************************************************************

extern sensor MPU9250;
float Qr[2][2]={9.47,0.17,0.17,25.5};				// 
float Qp[2][2]={11.12,1.57,1.57,13.3};				//  -> Matrices de covarianzas de ruido de planta
float Qy[2][2]={7.0,0.0,0.0,3.1};					// 
float Rr[2][2]={0.014,0.0,0.0,0.02};				//
float Rp[2][2]={0.014,0.0,0.0,0.02};				//  -> Matrices de covarianzas de ruido de medición
float Ry[2][2]={3.0,0.0,0.0,0.014};					//
float Pr[2][2]={0,0,0,0};							//  
float Pp[2][2]={0,0,0,0};							//  -> Matrices de covarianzas de error de estimación
float Py[2][2]={0,0,0,0};							//
float xr[2]={0,0}; 									//
float xp[2]={0,0};									//  -> Vectores de estados estimados
float xy[2]={0,0};									//
float accel[3]={0,0,0}; 							//	Arreglo aceleraciones [g]
float gyro[3]={0,0,0};								//	Arreglo velocidad angular [°/s]
float mag[3]={0,0,0};								//	Arreglo campo mágnetico [uT] 
float RPY[3]={0,0,0};								//	Arreglo ángulos RPY [°]
float ER[3]={0,0,0};								//	Arreglo tasas de Euler [°/s]
float magRef=0;										//
byte powerUpRef=1;									//	Estas tres variables son utilizadas para que el ángulo Yaw establezca su referencia con respecto
byte contador2=0;									//	a la orientación del robot al encender el sistema y no con respecto al norte (opcional)

//************************************************************************
//*						FUNCIONES Y PROCEDIMIENTOS                       *
//************************************************************************


/*
** ===================================================================
**     Función    :  ACQUIRE_DATA
**
**     Descripción :
**     Función para tomar los datos adquiridos, pasarlos a sus
**     unidades pertinentes y corregir el offset experimental. 
**         
**     Parámetros  : Ninguno. (Usa variables globales).
**     Retorna     : Nada. (Modifica variables globales)
** ===================================================================
*/
void ACQUIRE_DATA(void){
	
	accel[0]=(float)(MPU9250.dataFrame[0]-314)/16384;
	accel[1]=(float)(MPU9250.dataFrame[1]-145)/16384;
	accel[2]=(float)(MPU9250.dataFrame[2]-65)/16384;
	gyro[0]=(float)(MPU9250.dataFrame[3]-0)*125/8192;
	gyro[1]=(float)(MPU9250.dataFrame[4]-0)*125/8192;
	gyro[2]=(float)(MPU9250.dataFrame[5]-0)*125/8192;
	mag[0]=((float)((MPU9250.dataFrame[6]+28)*3))/30;
	mag[1]=((float)((MPU9250.dataFrame[7]+243)*3))/30;
	mag[2]=((float)((MPU9250.dataFrame[8]+82)*3))/30;;
}


/*
** ===================================================================
**     Función    :  EULER_RATES
**
**     Descripción :
**     Función utilizada para calcular las tres tasas de Euler
**              
**     Parámetros  : Ángulo pitch(radianes). (Támbién usa variables globales)
**     Retorna     : Nada. (Modifica variables globales)
** ===================================================================
*/
byte EULER_RATES(float pitch){
	
	float den= COS(pitch);
	byte errorCode=(byte)SUCCESS;
	if(den==0.0){
		errorCode=(byte)ERROR_DIV;
	}
	else{
		ER[1]=(1/den)*(accel[2]*gyro[1]-accel[1]*gyro[2]); // dp/dt
		ER[1]-= -0.25; // Corrección de offset
	}
	den= (1-accel[0]*accel[0]);
	if(den==0.0){
		errorCode=(byte)ERROR_DIV;
	}
	else{
		ER[2]= (1/den)*(accel[1]*gyro[1]+accel[2]*gyro[2]);// dy/dt
		ER[2]-= 0.84; // Corrección de offset
		ER[0]= gyro[0]-accel[0]*ER[2]; // dr/dt
		ER[0]-= -0.66; // Corrección de offset
	}
	return errorCode;
}

/*
** ===================================================================
**     Función     :  GET_PITCH
**
**     Descripción :
**     Función que permite el cómputo del ángulo PITCH
**         
**     Parámetros  : Ninguno. (Usa variables globales)
**     Retorna     : Nada. (Modifica variables globales)
** ===================================================================
*/

void GET_PITCH(void){
	
	float offset=-0.018; // Corrección de offset
	RPY[1]=ASIN(-accel[0])-offset;
}

void GET_ROLL(void){
	//Función que permite el cómputo del ángulo ROLL
	float offset=-0.075; // Corrección de offset
	if(accel[2]==0.0){
		if(accel[1]<0){
			RPY[0]= -PI/2;
		}
		else{
			RPY[0]= PI/2;
		}
	}
	
	else {
		RPY[0]=ATAN(accel[1]/accel[2])-offset;
	}
}


/*
** ===================================================================
**     Función     :  GET_YAW
**
**     Descripción :
**     Función que permite el cómputo del ángulo YAW
**
**     Parámetros  : Ninguno. (Usa variables globales)
**     Retorna     : Nada. (Modifica variables globales)
** ===================================================================
*/

void GET_YAW(void){
	float num,den;
	float offset=-0.104; // Corrección de offset
	num =(float)(-accel[2]*mag[1]+accel[1]*mag[2]);
	den =(float)(mag[0]+accel[0]*((-accel[1]*mag[1]-accel[2]*mag[2])-accel[0]*mag[0]));
	RPY[2]=ATAN2(num,den)-offset;
}

/*
** ===================================================================
**     Función     :  GET_RPY
**
**     Descripción :
**     Función para obtener los tres ángulos RPY
**
**     Parámetros  : Ninguno. 
**     Retorna     : Nada. 
** ===================================================================
*/

void GET_RPY(void){
	
	GET_ROLL();
	GET_PITCH();
	GET_YAW();
}

/*
** ===================================================================
**     Función     :  ADJUST_NOISE_MATRIX
**
**     Descripción :
**     Función para ajustar arbitrariamente las matrices de covarianza 
**     de ruido.
**
**     Parámetros  : 
**     
**     - R[][2]    -Matriz de covarianza del ruido de medición. (global)
**     - Q[][2]	   -Matriz de covarianza del ruido asociado a la planta. (global)
**     - factorQ   -Factor de ajuste de la matriz Q.
**     - factorR   -Factor de ajuste de la matriz R.
**      
**     Retorna     : Nada (Modifica variables globales). 
** ===================================================================
*/
void ADJUST_NOISE_MATRIX(float R[][2],float Q[][2],float factorQ,float factorR){
	Q[0][0]/=factorQ;
	Q[0][1]/=factorQ;
	Q[1][0]/=factorQ;
	Q[1][1]/=factorQ;
	R[0][0]*=factorR;
	R[0][1]*=factorR;
	R[1][0]*=factorR;
	R[1][1]*=factorR;
}


/*
** ===================================================================
**     Función     :  KALMAN_FILTER
**
**     Descripción :
**     Función que ejecuta el filtro de Kalman para un sólo ángulo.
**
**     Parámetros  : 
**     - data1	  	 -Ángulo de rotación.
**     - data2	  	 -Tasa de Euler o derivada del ángulo de rotación. 
**     - *xplus	   	 -Puntero al arreglo de estimación de estados (global).
**     - pplus[2][2] -Matriz de covarianza del error de estimación (global).
**     - R[2][2]   	 -Matriz de covarianza del ruido de medición.
**     - Q[2][2]     -Matriz de covarianza del ruido asociado a la planta. 
**      
**     Retorna     : 
** 	   - errorCode:  - SUCCESS si el filtro se ejecuto exitosamente.
** 	   				 - ERROR_DIV si hubo una división entre cero.
** ** ===================================================================
*/

byte KALMAN_FILTER(float data1,float data2,float* xplus,float pplus[2][2],float Q[2][2],float R[2][2]){
	float Z[2];
	float pminus[2][2];
	float xminus[2];
	float K[2][2];
	float determinante=1.0;
	byte errorCode=SUCCESS;
	Z[0]=data1;
	Z[1]=data2;
	// 1.Pk(-)=phi*Pk-1(+)*phi'+Q;
	pminus[0][0]= (float)(Q[0][0] + pplus[0][0] + dt*(pplus[1][0] + pplus[0][1] + dt*pplus[1][1])); 
	pminus[0][1]= (float)(Q[0][1] + pplus[0][1] + dt*pplus[1][1]);
	pminus[1][0]= (float)(Q[1][0] + pplus[1][0] + dt*pplus[1][1]);
	pminus[1][1]= Q[1][1] + pplus[1][1];
	//2.Kk= Pk(-)Hk^t[HkPk(-)Hk^t+Rk]^-1
	determinante= ((pminus[0][0]+R[0][0])*(pminus[1][1]+R[1][1])-(pminus[0][1]+R[0][1])*(pminus[1][0]+R[1][0]));
	if(determinante==0.0){
		errorCode=(byte)ERROR_DIV;
		return errorCode;
	}
	K[0][0]=(pminus[0][0]*(pminus[1][1]+R[1][1]) - pminus[0][1]*(pminus[1][0]+R[1][0]))/determinante;
	K[0][1]=(pminus[0][1]*R[0][0]-pminus[0][0]*R[0][1])/determinante;
	K[1][0]=(pminus[1][0]*R[1][1]-pminus[1][1]*R[1][0])/determinante;
	K[1][1]=(pminus[1][1]*(R[0][0]+pminus[0][0]) - pminus[1][0]*(R[0][1]+pminus[0][1]))/determinante;
	// 3.Pk(+)= (1-Kk*H)Pk(-)
	pplus[0][0]= pminus[0][0]*(1-K[0][0])- K[0][1]*pminus[1][0];
	pplus[0][1]= pminus[0][1]*(1-K[0][0])- K[0][1]*pminus[1][1];
	pplus[1][0]= pminus[1][0]*(1-K[1][1])- K[1][0]*pminus[0][0];
	pplus[1][1]= pminus[1][1]*(1-K[1][1])- K[1][0]*pminus[0][1];
	// 4.Xk(-)= phi*Xk-1(+)
	xminus[0]=(float)(xplus[0]+ dt*xplus[1]);
	xminus[1]= xplus[1];
	// 5.Xk(+)=Xk(-)+Kk*(Zk-Hk*Xk(-))
	xplus[0]= xminus[0]-K[0][0]*(xminus[0]-Z[0])-K[0][1]*(xminus[1]-Z[1]);
	xplus[1]= xminus[1]-K[1][0]*(xminus[0]-Z[0])-K[1][1]*(xminus[1]-Z[1]);
	// FIN
	return errorCode;
}
