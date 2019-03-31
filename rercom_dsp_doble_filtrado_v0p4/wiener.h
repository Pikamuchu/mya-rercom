/***************************************************************************
                          wiener.h  -  description
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

#include <math.h>
#include "definiciones.h"
#include "funciones.h"  

typedef struct{
  
  int    longitud_trama;          // Longitud de las tramas
  int    orden_predictor_sonoras; // Orden del predictor sonora; calculo espectro modelado
  int    orden_predictor_sordas;  // Orden del predictor sordas; calculo espectro modelado  
  int    tipo_prefiltro;             // Tipo de prefiltro
  int    tipo_filtro;             // Tipo de filtro
  float  umbral_pitch;            // Sobrestimacion umbral pitch
  float  pre_beta;                // Parametro beta prefiltro wiener
  float  pre_delta;               // Parametro delta prefiltro wiener
  float  beta;                    // Parametro beta filtro wiener
  float  delta;                   // Parametro delta filtro wiener
  float  aten_filtro;             // Atenuación maxima del filtro
  float  aten_prefiltro;          // Atenuación maxima del prefiltro  
  float  nivel_ruido;             // Nivel de ruido en tramas de silencio  
  float  alfa;                    // Parametro sobreestimacion ruido en VAD
  int    num_tramas_ruido;        // Numero de tramas iniciales de ruido
  int    numero_trama;            // Numero de trama para calculo del coef de promediado de ruido
  float  gamma_ruido;             // Parametro de promediado espectro ruido
  int    reestimar_ruido;         // Flag de reestimación de ruido en tramas silencio 
  float  gamma_senal;             // Parametro de promediado espectro senal
  
  float  *trama;                  // trama auxiliar para filtro de wiener

  float  *dft_real;               // parte real transformada fourier trama entrada 
  float  *dft_imag;               // parte imaginaria transformada fourier trama entrada 

  float  *espectro_voz_ruido;     // espectro voz + ruido
  float  *espectro_ruido;         // espectro ruido promediado
  
  float  potencia_ruido;
  float  potencia_voz_ruido;

  float  *correlaciones;          // correlaciones calculadas
  float  *aks;                    // coeficientes ak calculados
  float  *espectro_modelado;      // espectro voz (modelado AR) 
  
  float  atenuacion;
  float  *dft_modulo_filtro;
  
  float  umbral_VAD;
  int    tramas_guarda;
  
  int corte_bajo;
  int corte_alto;
  
}Wiener;

/*constructor-destructor de objeto*/

Wiener *wiener_nuevo(int longitud_trama,
                     int orden_predictor_sonoras,
                     int orden_predictor_sordas,
                     int tipo_prefiltro,
                     float pre_beta,
                     float pre_delta,                     
                     int tipo_filtro,
                     float beta,
                     float delta,
                     float umbral_pitch,
                     float aten_prefiltro,
                     float aten_filtro,
                     float nivel_ruido,
                     int num_tramas_ruido,
                     float alfa,
                     float gamma_ruido,
                     int reestimar_ruido,
                     float gamma_senal,
                     int frec_muestreo);

void wiener_borrar(Wiener *wiener);

/*funciones de tratamiento de objeto */

int filtrado_wiener(Wiener *wiener,
                    float *trama_in,
                    float *trama_out);

