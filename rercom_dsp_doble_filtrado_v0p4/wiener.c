/***************************************************************************
                          main.c  -  description
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

#include "wiener.h"

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
                     int frec_muestreo){
                     
  int reserva = sizeof(float)*longitud_trama;
  float tmp;
                     
  Wiener *wiener = malloc(sizeof(Wiener));
  wiener->longitud_trama = longitud_trama;
  wiener->orden_predictor_sonoras = orden_predictor_sonoras;
  wiener->orden_predictor_sordas = orden_predictor_sordas;  
  wiener->tipo_prefiltro = tipo_prefiltro;
  wiener->pre_beta = pre_beta;
  wiener->pre_delta = pre_delta;
  wiener->tipo_filtro = tipo_filtro;
  wiener->beta = beta;
  wiener->delta = delta;
  wiener->umbral_pitch = umbral_pitch;
  wiener->aten_prefiltro = aten_prefiltro;  
  wiener->aten_filtro = aten_filtro;
  wiener->nivel_ruido = nivel_ruido;
  wiener->num_tramas_ruido = num_tramas_ruido;
  wiener->numero_trama = 0;
  wiener->alfa = alfa;
  wiener->gamma_ruido = gamma_ruido;
  wiener->reestimar_ruido = reestimar_ruido;
  wiener->gamma_senal = gamma_senal;

  wiener->trama = malloc(reserva);
  wiener->dft_real = malloc(reserva); 
  wiener->dft_imag = malloc(reserva);
  
  wiener->espectro_voz_ruido = malloc(reserva);
  wiener->espectro_ruido = malloc(reserva);

  wiener->correlaciones = malloc(reserva);
  wiener->aks = malloc(sizeof(float)*(orden_predictor_sonoras+1));
  wiener->espectro_modelado = malloc(reserva);
  
  wiener->potencia_voz_ruido = 0;
  wiener->potencia_ruido = 0;
  
  wiener->atenuacion = 1;
  wiener->dft_modulo_filtro = malloc(reserva);
  
  wiener->umbral_VAD = 0;
  wiener->tramas_guarda = MAX_GUARDA;
  
  // calculo frecuencias de corte en muestras 
  // CORTE_BAJO Y CORTE_ALTO son las de referencia calculadas a 8Khz/256muestras
  tmp = CORTE_BAJO;
  tmp *= longitud_trama;
  tmp /= LONGITUD_TRAMA;
  wiener->corte_bajo = (int)tmp;
  tmp = CORTE_ALTO;
  tmp *= frec_muestreo;
  tmp /= FREC_MUESTREO_REFERENCIA;
  tmp *= longitud_trama;
  tmp /= LONGITUD_TRAMA;
  wiener->corte_alto = (int)tmp;
 
  return(wiener);

}

void wiener_borrar(Wiener *wiener){
  
  free(wiener->trama);
  free(wiener->dft_real);
  free(wiener->dft_imag);
  free(wiener->espectro_voz_ruido);
  free(wiener->espectro_ruido);
  free(wiener->correlaciones);
  free(wiener->aks);  
  free(wiener->espectro_modelado);
  free(wiener->dft_modulo_filtro);  
  free(wiener);

}    

int filtrado_wiener(Wiener *wiener,
                    float *trama_in,
                    float *trama_out){
                         
/****************** inicialización variables locales **************************/

  int j;
  int silencio = CIERTO;
  int orden = wiener->orden_predictor_sonoras;
  float pitch = 0;
  float distancia = 0;
  float promedio = 0;
  float nivel = 0;
  
/******************************************************************************/

/********************** enventanado y transformada de Fourier *****************/    

  // realizamos el enventanado de la trama para el calculo de estimaciones
  enventanar(wiener->trama,
             trama_in,
             wiener->longitud_trama);
 
  // realizamos la transformada rapida de fourier
  fft(wiener->dft_real,
      wiener->dft_imag,
      wiener->trama,
      wiener->longitud_trama);
      
/******************************************************************************/      

  // condicion prefiltro activado (0 -> desactivado)
  if(wiener->tipo_prefiltro>0){

/*********** Estimación de la señal de voz con ruido para prefiltrado *********/

  estimar_espectro(wiener->dft_real,
                   wiener->dft_imag,
                   wiener->espectro_voz_ruido,
                   wiener->longitud_trama);
    
  wiener->potencia_voz_ruido = potencia_espectro(wiener->espectro_voz_ruido,
                                                 wiener->longitud_trama);  
                                     
/******************************************************************************/

/***************************** PREFILTRO **************************************/

  // condicion inicial-> aun no hay estimacion de ruido 
  if(wiener->numero_trama==0){
    // Construcción filtro de Wiener -> H(w)
    construir_filtro(wiener->dft_modulo_filtro,
                     wiener->espectro_modelado,
                     wiener->espectro_voz_ruido,
                     wiener->espectro_voz_ruido,
                     wiener->longitud_trama,
                     wiener->aten_prefiltro,
                     0, //no se utiliza el pitch
                     wiener->corte_bajo,
                     wiener->corte_alto,
                     wiener->tipo_prefiltro,
                     wiener->pre_beta,
                     wiener->pre_delta);

  }else{
    // Construcción filtro de Wiener -> H(w)
    construir_filtro(wiener->dft_modulo_filtro,
                     wiener->espectro_modelado,
                     wiener->espectro_ruido,
                     wiener->espectro_voz_ruido,
                     wiener->longitud_trama,
                     wiener->aten_prefiltro,
                     0,  // no se utiliza el pitch
                     wiener->corte_bajo,
                     wiener->corte_alto,
                     wiener->tipo_prefiltro,
                     wiener->pre_beta,
                     wiener->pre_delta);
  }
      
  filtrar(wiener->dft_real,
          wiener->dft_imag,
          wiener->dft_modulo_filtro,
          wiener->longitud_trama);

/******************************************************************************/

  }// fin del if prefiltro
  
/************ Estimación de la señal de voz con ruido para filtrado ***********/

    estimar_espectro(wiener->dft_real,
                     wiener->dft_imag,
                     wiener->espectro_voz_ruido,
                     wiener->longitud_trama);
    
    wiener->potencia_voz_ruido = potencia_espectro(wiener->espectro_voz_ruido,
                                                   wiener->longitud_trama);  
                                     
/******************************************************************************/

/******************************* VAD ******************************************/

  // Condicion para fase de aprendizaje del VAD        
  if(wiener->numero_trama < wiener->num_tramas_ruido){

    // aprendizaje umbral VAD basado en distancia espectral    
    distancia = distancia_espectral(wiener->espectro_ruido,
                                    wiener->potencia_ruido,
                                    wiener->espectro_voz_ruido,
                                    wiener->potencia_voz_ruido,
                                    wiener->longitud_trama);

    // la distancia umbral es la maxima obtenida en la fase de aprendizaje
    if(wiener->umbral_VAD < distancia){
      wiener->umbral_VAD = distancia;
    }
 
    // Trama ruido (aprendizaje)  
    silencio=CIERTO;
    wiener->tramas_guarda=0;
 
  }else{
    
    // VAD basado en potencia 
    if(wiener->potencia_voz_ruido < (wiener->alfa * wiener->potencia_ruido)){
          
      // VAD basado en distancia espectral    
      distancia = distancia_espectral(wiener->espectro_ruido,
                                      wiener->potencia_ruido,
                                      wiener->espectro_voz_ruido,
                                      wiener->potencia_voz_ruido,
                                      wiener->longitud_trama);
    
      if(distancia < (wiener->alfa * wiener->umbral_VAD)){

        if(wiener->tramas_guarda<=0){
          // Trama ruido 
          silencio=CIERTO;
          wiener->tramas_guarda=0;
        }else{
          silencio=FALSO;
          wiener->tramas_guarda--;
        }
        
      }else{
        // Trama de voz
        silencio=FALSO;
        wiener->tramas_guarda = MAX_GUARDA;
      }
    
    }else{
  
      // Trama de voz
      silencio=FALSO;
      wiener->tramas_guarda = MAX_GUARDA;
    
    }

  }

/******************************************************************************/

/************************** Estimacion pitch **********************************/
          
  // parte solo necesaria para el filtro wiener tipo 1 
  if((wiener->tipo_filtro==WIENER_1)&&(wiener->umbral_pitch>0)){
  
    // si es una trama de ruido (silencio = CIERTO) no calculamos pitch
    if(silencio){
      pitch=0;
    }else{
      // condicion de pitch desactivado
      if(wiener->umbral_pitch > 0){
        pitch=estimar_pitch(wiener->trama,
                            wiener->longitud_trama,
                            wiener->umbral_pitch);
      }
    }
    
  }
        
/******************************************************************************/        

  // Condicion de filtrar o atenuar la trama de entrada 
  if(silencio){

/**************** Estimación de ruido para prefiltrado/filtrado ***************/

    // condicion reestimacion ruido o estimacion inicial de ruido
    if((wiener->reestimar_ruido) || 
       (wiener->numero_trama < wiener->num_tramas_ruido)){

      // Calculo del promediado a utilizar
      promedio = 1;
      promedio /= (wiener->numero_trama+1);
      if(promedio < wiener->gamma_ruido) promedio = wiener->gamma_ruido; 
  
      promediar_espectro(wiener->espectro_voz_ruido,
                         wiener->espectro_ruido,
                         promedio,
                         wiener->longitud_trama);
 
      wiener->potencia_ruido = potencia_espectro(wiener->espectro_ruido,
                                                 wiener->longitud_trama);

      // Calculo nivel de atenuacion del filtrado
      if(wiener->potencia_ruido > wiener->nivel_ruido){
        nivel = (float)sqrt(wiener->nivel_ruido/wiener->potencia_ruido);
      }else{
        nivel = 1;
      }
      
      // para asegurarmos que no se sobrepasa la atenuación máxima     
      if(nivel > wiener->aten_filtro){
        wiener->atenuacion = nivel;
      }else{
        wiener->atenuacion = wiener->aten_filtro;
      }

      wiener->numero_trama++;
      
    }
      
/******************************************************************************/          

/***************************** atenuador **************************************/

    // condicion activacion filtro
    if(wiener->tipo_filtro>0){ 

      atenuar(wiener->dft_real,
              wiener->dft_imag,
              wiener->atenuacion,
              wiener->longitud_trama);
              
    }

/******************************************************************************/
  
  }else{
  
    // condicion activacion filtro
    if(wiener->tipo_filtro>0){ 

/*********** Estimación de la señal modelada para filtrado ********************/

    // parte solo necesaria para el filtro wiener tipo 1 
    if(wiener->tipo_filtro==WIENER_1){
  
      // Eleccion orden predictor en funcion de trama sonora o 
      // trama sorda/ruido
      if(pitch==0){
        orden = wiener->orden_predictor_sordas;
      }else{
        orden = wiener->orden_predictor_sonoras;
      }

      correlacion(wiener->trama,
                  wiener->correlaciones,
                  wiener->longitud_trama,
                  orden);
      
      obtener_ak_levinson_durvin(wiener->aks,
                                 wiener->correlaciones,
                                 orden);        

      espectro_modelado(wiener->espectro_modelado,
                        wiener->aks,
                        wiener->potencia_voz_ruido,
                        wiener->potencia_ruido,
                        wiener->longitud_trama,
                        orden);

    }

/******************************************************************************/

/*************************** filtrado *****************************************/

    // Construcción filtro de Wiener -> H(w)
    construir_filtro(wiener->dft_modulo_filtro,
                     wiener->espectro_modelado,
                     wiener->espectro_ruido,
                     wiener->espectro_voz_ruido,
                     wiener->longitud_trama,
                     wiener->atenuacion,
                     pitch,
                     wiener->corte_bajo,
                     wiener->corte_alto,
                     wiener->tipo_filtro,
                     wiener->beta,
                     wiener->delta);
      
    filtrar(wiener->dft_real,
            wiener->dft_imag,
            wiener->dft_modulo_filtro,
            wiener->longitud_trama);

/******************************************************************************/

    }//fin del if de filtro activo 
   
  }//fin de if atenuar o filtrar

  ifft(trama_out,
       wiener->dft_real,
       wiener->dft_imag,
       wiener->longitud_trama);

   
  return(OK);

}
