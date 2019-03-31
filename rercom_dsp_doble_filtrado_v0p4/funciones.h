/***************************************************************************
                          utilidades.h  -  description
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
  
int fft(float *real,
        float *imag,
        float *muestras,
        int lon);

int ifft(float *muestras,
         float *real,
         float *imag,
         int lon);

int estimar_espectro(float *real,
                     float *imag,
                     float *espectro,
                     int lon);

int promediar_espectro(float *espectro,
                       float *espectro_ant,
                       float gamma,
                       int lon);

int correlacion(float *muestras,
                float *correlaciones,
                int long_trama,
                int orden);

int obtener_ak_levinson_durvin(float *aks,
                               float *correlaciones,
                               int orden);

float estimar_pitch(float *muestras,
                    int longitud_trama,
                    float sobreumbral);

float distancia_espectral(float *espectro_ruido,
                          float pot_ruido,
                          float *espectro_senal,
                          float pot_senal,
                          int lon);

int enventanar(float *muestras_out,
               float *muestras_in,
               int longitud_trama);

float potencia_espectro(float *espectro,
                        int lon);
  
float construir_filtro(float *dft_modulo_filtro,
                       float *espectro_modelado,
                       float *espectro_ruido,
                       float *espectro_voz_ruido,
                       int lon,
                       float atenuacion,
                       float pitch,
                       int corte_bajo,
                       int corte_alto,
                       int tipo_filtro,
                       float beta,
                       float delta);

int filtrar(float *dft_real,
            float *dft_imag,
            float *dft_modulo_filtro,
            int lon);

int atenuar(float *dft_real,
            float *dft_imag,
            float aten,
            int lon);
            
int espectro_modelado(float *espectro,
                      float *aks,
                      float pot_senal,
                      float pot_ruido,
                      int long_trama,
                      int orden);
            

