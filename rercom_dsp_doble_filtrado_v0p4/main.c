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

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "definiciones.h"
#include "senal.h"
#include "wiener.h"

int main(int argc, char *argv[]){

/************** inicialización variables del main *****************************/

  int i,err,reserva,numero_tramas;
  float *trama_in;
  float *trama_out;
  char fichero_in[256];
  char fichero_out[256];
  int formato=WAV;
  int frec_muestreo=FREC_MUESTREO_REFERENCIA;
  int longitud_trama = LONGITUD_TRAMA;
  int desplaza_trama = DESPLAZA_TRAMA;
  float tmp;
  
  Senal *senal;
  Wiener *wiener;
  
/******************************************************************************/

/************************ Acerca de proyecto rercom ***************************/

  printf("\nProyecto RERCOM DSP doblefiltrado version 0.4\n");
  printf("\nCopyright (C) 2004 by:\n");
  printf("\n    Antonio Marin  (amar5783@etsetb.upc.es)");
  printf("\n    Marta del Pozo (mdel5525@etsetb.upc.es)");  
  printf("\n");

/******************************************************************************/
  
/********** lectura de los parametros de configuracion del sistema ************/

  if(argc == 3){
    strcpy(fichero_in,argv[1]);
    strcpy(fichero_out,argv[2]);
    formato=WAV;
  }else{
    if(argc == 5){
      strcpy(fichero_in,argv[1]);
      strcpy(fichero_out,argv[2]);
      formato=atoi(argv[3]);
      frec_muestreo=atoi(argv[4]);
      longitud_trama = (int)LONGITUD_TRAMA*(frec_muestreo/FREC_MUESTREO_REFERENCIA);
      desplaza_trama = (int)DESPLAZA_TRAMA*(frec_muestreo/FREC_MUESTREO_REFERENCIA);
   }else{
      printf("\nFiltrado archivos WAV:\n");
      printf("\n  rercom arg1 arg2\n");
      printf("\n    donde arg1: nombre del fichero de entrada");
      printf("\n          arg2: nombre del fichero de salida");   
      printf("\n\nFiltrado archivos RAW:\n");
      printf("\n  rercom arg1 arg2 arg3 arg4\n");
      printf("\n    donde arg1: nombre del fichero de entrada");
      printf("\n          arg2: nombre del fichero de salida");   
      printf("\n          arg3: tipo formato: 1->INTEL");
      printf("\n                              2->MOTOROLA");            
      printf("\n          arg4: frecuencia de muestreo en Hz");   
      printf("\n");
      exit(-1);
    }
  }

/******************************************************************************/
  
/********************** lectura del fichero de señal **************************/
  
  senal = senal_nuevo(fichero_in,
                      longitud_trama,
                      desplaza_trama,
                      frec_muestreo);
 
  switch(formato){
  
    case WAV:

      printf("\nLeyendo fichero de senyal en formato WAV\n");
   
      err=leer_fichero_senal_WAV(senal,
                                 fichero_in);

      // Error por WAV incompatible o no se puede abrir archivo 
      if(err==ERROR2) exit(-1);

      // Error archivo no es un WAV
      if(err==ERROR){
        printf("\n\nError al intentar leer el fichero %s en formato WAV",fichero_in);

        exit(-1);
      }
      
      // ajustamos parametros por si la frecuencia de muestreo no es la de referencia
      if(senal->frec_muestreo != FREC_MUESTREO_REFERENCIA){
        tmp = LONGITUD_TRAMA;
        tmp *= senal->frec_muestreo;
        tmp /= FREC_MUESTREO_REFERENCIA;
        longitud_trama = (int)tmp;
        //printf("\n longitud trama es %d %f",longitud_trama,tmp);
        tmp = DESPLAZA_TRAMA;
        tmp *= senal->frec_muestreo;
        tmp /= FREC_MUESTREO_REFERENCIA;
        desplaza_trama = (int)tmp;
        //printf("\n desplaza trama es %d %f",desplaza_trama,tmp);
        senal->longitud_trama = longitud_trama;
        senal->desplaza_trama = desplaza_trama;
      }
        
      break;

    case RAW_INTEL:
    
      printf("\nLeyendo fichero de senyal en formato RAW_INTEL\n");

      err=leer_fichero_senal_RAW(senal,
                                 fichero_in,
                                 formato);

      // Error archivo no es un WAV
      if(err==ERROR){
        printf("\n\nError al intentar leer el fichero %s en formato RAW_INTEL",fichero_in);
        exit(-1);
      }
  
      break;

    case RAW_MOTOROLA:
    
      printf("\nLeyendo fichero de senyal en formato RAW_MOTOROLA\n");

      err=leer_fichero_senal_RAW(senal,
                                 fichero_in,
                                 formato);

      // Error archivo no es un WAV
      if(err==ERROR){
        printf("\n\nError al intentar leer el fichero %s en formato RAW_MOTOROLA",fichero_in);
        exit(-1);
      }
  
      break;
      
    default:
    
      printf("\n\nFormato de archivo no valido (%d)",formato);
      exit(-1);
      
      break;

  }
  
/******************************************************************************/

/*************** Inicializacion estructuras ***********************************/

  reserva = sizeof(float)*longitud_trama;

  trama_in=malloc(reserva);
  trama_out=malloc(reserva);

  wiener = wiener_nuevo(longitud_trama,
                        ORDEN_PREDICTOR_SONORAS,
                        ORDEN_PREDICTOR_SORDAS,
                        WIENER_2,
                        PRE_BETA,
                        PRE_DELTA,
                        WIENER_1,
                        BETA,
                        DELTA,
                        UMBRAL_PITCH,
                        MAX_ATEN_PREFILTRO,                        
                        MAX_ATEN_FILTRO,
                        NIVEL_RUIDO,
                        NUM_TRAMAS_RUIDO,
                        ALFA,
                        GAMMA_RUIDO,
                        REESTIMAR_RUIDO,
                        GAMMA_SENAL,
                        senal->frec_muestreo);


/******************************************************************************/
  
/********** Calculo parametros bucle principal filtro wiener ******************/
  
  // Calculo numero de tramas de senal 
  numero_tramas = (int)(senal->longitud/desplaza_trama);
  numero_tramas -= (int)(longitud_trama/desplaza_trama);
  numero_tramas++;

/******************************************************************************/ 

/********* Bucle principal Filtrado wiener con prefiltrado ********************/

  printf("\n");

  for(i=0;i<numero_tramas;i++){
  
    // visualizacion de porcentaje de filtrado acabado
    //printf("\rFiltrando fichero: %d%% hecho",(i*100)/(numero_tramas-1) );

/*********** Obtencion de la trama para calculo de espectros ******************/
    
    // Obtenemos trama de señal con ruido -> y(n)
    err=obtener_trama(senal,
                      trama_in,
                      i);
    if(err!=OK){
      printf("\nError en main -> obtener_trama");
      exit(ERROR);
    }
    
/******************************************************************************/      

/***************************** filtrado de wiener *****************************/

    err=filtrado_wiener(wiener,
                        trama_in,
                        trama_out);
    if(err!=OK){
      printf("\nError en main -> filtrado_wiener");
      exit(ERROR);
    }

/******************** Reconstruccion señal de voz  ****************************/

    err=guardar_senal(senal,
                      trama_out,
                      i,
                      0);
    if(err!=OK){
      printf("\nError en main -> guardar_senal");
      exit(ERROR);
    }

/******************************************************************************/
   
  }// fin del bucle principal
  
/*************************** Escritura fichero salida *************************/    

  switch(formato){
  
    case WAV:
  
      // Escribimos el fichero en formato WAV de la senyal filtrada
      err=escribir_fichero_senal_WAV(senal,
                                     fichero_out);

      // Error por WAV incompatible o no se puede abrir archivo 
      if(err==ERROR2) exit(-1);

      // Error archivo no es un WAV
      if(err==ERROR){
        printf("\n\nError al intentar leer el fichero %s en formato WAV",fichero_in);
        exit(-1);
      }
      
    break;


    case RAW_INTEL:

      // Escribimos el fichero en formato WAV de la senyal filtrada
      err=escribir_fichero_senal_RAW(senal,
                                     fichero_out,
                                     formato);

      // Error archivo no es un WAV
      if(err==ERROR){
        printf("\n\nError al intentar leer el fichero %s en formato RAW_INTEL",fichero_in);
        exit(-1);
      }
  
    break;

    case RAW_MOTOROLA:

      // Escribimos el fichero en formato WAV de la senyal filtrada
      err=escribir_fichero_senal_RAW(senal,
                                     fichero_out,
                                     formato);

      // Error archivo no es un WAV
      if(err==ERROR){
        printf("\n\nError al intentar leer el fichero %s en formato RAW_MOTOROLA",fichero_in);
        exit(-1);
      }
  
    break;
      
    default:
    
      printf("\n\nFormato de archivo no valido (%d)",formato);
      exit(-1);
      
      break;

  }

/******************************************************************************/
 
  wiener_borrar(wiener);
  senal_borrar(senal);
  free(trama_in);
  free(trama_out);      

  exit(OK);

}
