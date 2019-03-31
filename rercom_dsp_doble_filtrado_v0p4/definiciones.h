/***************************************************************************
                          definiciones.h  -  description
                             -------------------
    begin                : vie feb 27 2004
    copyright            : (C) 2004 by Antonio Marin y Marta del Pozo
    email                : amar5783@alu-etsetb.upc.es
                           mdel5525@alu-etsetb.upc.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
/************************* Constantes varias  ******************************/

#define PI               3.14159265358
#define PI2              6.283185307179586
#define CIERTO           1
#define FALSO            0
#define OK               0
#define NULO             0
#define ERROR           -1
#define ERROR2          -2
#define VALOR_VAD        10000

/***************************************************************************/

/************************* Parametros programa *****************************/

#define LONGITUD_TRAMA 256 // numero de muestras de la trama
#define DESPLAZA_TRAMA 128 // desplazamiento en muestras entre tramas (50% solape)
#define ORDEN_PREDICTOR_SONORAS 8 // numero coef ak para trama sonora (pitch detectado)
#define ORDEN_PREDICTOR_SORDAS 3 // numero coef ak para trama sorda (pitch no detectado)
#define PRE_BETA 1.2 // parametro prefiltro; sobreestimacion ruido
#define PRE_DELTA 2 // parametro prefiltro; efecto no linial
#define BETA 1.2 // parametro filtro; sobreestimacion ruido
#define DELTA 1 // parametro filtro; efecto no linial
#define UMBRAL_PITCH 0.5 // subumbral detector pitch (0->desactivado)
#define MAX_ATEN_PREFILTRO 0.5 // atenuación máxima del prefiltro
#define MAX_ATEN_FILTRO 0.01 // atenuación máxima del filtro
#define NIVEL_RUIDO 2500 // energia de ruido residual (calculado a ojo)
#define NUM_TRAMAS_RUIDO 5 // numero de tramas iniciales de ruido
#define ALFA 1.15 // parametro sobreestimacion umbral VAD
#define GAMMA_RUIDO 0.1 // tipo de promedio para estimacion ruido (1 -> desactivado)  
#define REESTIMAR_RUIDO 1 // 1 -> restimar; 0 -> no reestimar
#define GAMMA_SENAL 1 // tipo de promedio para estimacion voz+ruido (1 -> desactivado)  
#define FREC_MUESTREO_REFERENCIA 8000 // frec a los que estan referidos los parametros
#define MAX_GUARDA 2 // numero de tramas que se descartan antes de estimar ruido
// frecuencia de corte = ((muestra_corte*31,25)+15,625) hz filtro paso alto (8Khz L=256)
// (pitch voz hombre 110 hz, mujer 220 hz, niño 300 Hz ) 
#define CORTE_BAJO    2 // 78,125 hz
// frecuencia de corte = (muestra_corte*31,25)-15,625) hz filtro paso bajo (8Khz L=256)
#define CORTE_ALTO  110 // 3453,125 hz
// rango de pitch valido
#define  MAX_MUESTRAS_PITCH  100 // equivale a 80Hz; f = Fm/n = 8000/100 = 80
#define  MIN_MUESTRAS_PITCH  20 // equivale a 400Hz; f = Fm/n = 8000/20 = 400  


/***************************************************************************/

/******************* Constantes caracteristicas senyal *********************/

// Numero de bytes en funcion del tipo de muestra
#define BITS_8_PCM            1
#define BITS_16_PCM           2
#define BITS_24_PCM           3
#define BITS_32_PCM           4

// Formato muestra (solo para 8 i 16 bits PCM)
#define UNSIGNED         0
#define SIGNED           1

// Canales
#define MONO             1
#define STEREO           2

/***************************************************************************/

/**************** Definicion de tipos de formato ficheros ******************/

#define WAV            0
#define RAW_INTEL      1
#define RAW_MOTOROLA   2

/**************************************************************************/

/******************** Definicion niveles estadisticas *********************/

#define NIVEL0      0
#define NIVEL1      1
#define NIVEL2      2

/**************************************************************************/

/******************* Definicion de tipos de ventanas ***********************/

#define RECTANGULAR      1
#define HAMMING          2
#define HANNING          3
#define KAISER           4
#define NUTTALL          5
#define BLACKMAN         6
#define HARRIS           7
#define BARTLETT         8
#define BARTHANN         9
#define PAPOULIS         10
#define GAUSS            11
#define PARZEN           12
#define HANNA            13
#define DOLPH            14
#define NUTBESS          15
#define SPLINE           16

/**************************************************************************/

/**************** Definicion de tipos de estadistica **********************/

#define CORRELACION      1
#define CUMULANTE_2      2
#define CUMULANTE_3      3
#define CUMULANTE_4      4

/**************************************************************************/

/**************** Definicion tipos de filtrados ***************************/

#define NO_FILTRAR   0     
#define WIENER_1     1     // H(w) = Ps(w) / (Ps(w) + Pd(w)) 
#define WIENER_2     2     // H(w) = (Px(w) - Pd(w)) / Px(w) 

/**************************************************************************/

/**************** Definicion tipos de postfiltrados ***********************/

#define NO_FILTRAR   0     
#define MEDIANA      1     // Filtrado de mediana 
#define MEDIANA2     2     // Filtrado de mediana sin efecto saturacion 
/**************************************************************************/

/**************** Definicion tipos de tramas de voz ***********************/

#define RUIDO        0     
#define SONORA       1     
#define SORDA        2     
/**************************************************************************/

