/***************************************************************************
                          senal.h  -  description
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


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct{
  
  int             longitud;	         // Longitud total de la señal a filtrar 
  int             longitud_trama;    // numero muestras de las tramas
  int             desplaza_trama;    // desplazamiento entre tramas
  short          *muestras;          // muestras de la señal a filtrar 
  short          *muestras_filtradas;// muestras filtradas de la señal 
  short          *VAD;               // indicador VAD 
  int             frec_muestreo;	   // Frecuencia de muestreo de la señal en Hz 
  int             tipo_muestra;      // numero de bits por muestra i codificacion
  int             formato_muestra;   // si las muestras son SIGNED o UNSIGNED
  int             canales;           // canales de audio del archivo

}Senal;

/*constructor-destructor de objeto*/

Senal* senal_nuevo(char *nombre_fichero1, int long_trama, int dt, int fm);

void senal_borrar(Senal *senal);

/*funciones de tratamiento de objeto */

int obtener_trama(Senal *senal, float *muestras, int numero_trama);

int leer_fichero_senal_RAW(Senal *senal,char *nombre_fichero1,int tipo);

int leer_fichero_senal_WAV(Senal *senal,char *nombre_fichero1);

int escribir_fichero_senal_RAW(Senal *senal,char *nombre_fichero2,int tipo); 

int escribir_fichero_senal_WAV(Senal *senal,char *nombre_fichero2);   

int escribir_fichero_VAD(Senal *senal,char *nombre_fichero3);

int guardar_senal(Senal *senal, float *muestras_filtradas, int numero_trama, int vad);

