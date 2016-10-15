/*
 * EmbeddedKalman.h
 * Descripción: El siguiente archivo contiene las variables y funciones 
 * utilizadas para:
 * 		- Cálculo de los ángulos RPY.	
 *      - La ejecución del filtro de Kalman embebido del INS.
 * 
 * GID Mecatrónica. Universidad Simón Bolívar. Venezuela. 
 *  Creado en: Aug 7, 2016
 *      Autores: Bruno Barone y Jhonny Rodríguez.
 */

#ifndef EMBEDDEDKALMAN_H_
#define EMBEDDEDKALMAN_H_

#include "Functions.h"
#define dt 0.010 // Tiempo de muestreo


void ACQUIRE_DATA(void);
void GET_ROLL(void);
void GET_PITCH(void);
void GET_YAW(void);
void GET_RPY(void);
void ADJUST_NOISE_MATRIX(float R[][2],float Q[][2],float factorQ,float factorR);
byte EULER_RATES(float pitch);
byte KALMAN_FILTER(float data1,float data2,float *xplus,float pplus[2][2],float Q[2][2],float R[2][2]);



#endif /* EMBEDDEDKALMAN_H_ */
