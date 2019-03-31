/***************************************************************************
                          senal.c  -  description
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


#include "senal.h"
#include "definiciones.h"
#include "funciones.h"

Senal *senal_nuevo(char *nombre_fichero1, int long_trama, int dt, int fm){

  FILE *f;
  Senal *senal = malloc(sizeof(Senal));
  register int tmp;
  int i;

  // Abre el fichero de datos
  f=fopen(nombre_fichero1,"rb");
  if(f == NULO){
    printf("\nNo se puede abrir el fichero %s\n",nombre_fichero1);
    return(NULL);
  }
  
  // Obtiene el tamaño del fichero de datos señal
  fseek (f , 0 , SEEK_END);
  tmp = ftell (f);
  
  //Calculo para hacer el tamaño de fichero divisible por longitud trama
  tmp /= long_trama;
  tmp++;
  tmp *= long_trama;
  senal->longitud = tmp;
  senal->longitud_trama = long_trama;
  senal->desplaza_trama = dt;
    
  // Reserva memoria para cargar todo el fichero (divisible por longitud trama)
  senal->muestras = malloc(tmp);
  senal->muestras_filtradas = malloc(tmp);
  senal->VAD = malloc(tmp);
  
  tmp/=2;
  for(i=0;i<tmp;i++){
    *(senal->muestras_filtradas + i) = 0;
  }
  
  // Parametros por defecto fichero RAW
  senal->frec_muestreo = fm;
  senal->tipo_muestra = BITS_16_PCM;
  senal->formato_muestra = SIGNED;
  senal->canales = MONO;
  
  fclose(f);
  
  return(senal);
}

void senal_borrar(Senal *senal){

  free(senal->muestras);
  free(senal->muestras_filtradas);
  free(senal->VAD);
  free(senal);

}

int obtener_trama(Senal *senal, float *muestras, int numero_trama){

  int i,desplaza;
  int long_trama = senal->longitud_trama;
  register float tmp;

  // calculo desplazamiento  
  desplaza=numero_trama*senal->desplaza_trama;
      
  for(i=0;i<long_trama;i++){
      
    tmp = (float)(*(senal->muestras + desplaza + i));
    *(muestras + i) = tmp;
     
  }
  
  return(OK);

}


int leer_fichero_senal_RAW(Senal *senal,char *nombre_fichero1,int tipo){

  int i, lon;
  short tmp,aux;
  FILE *f;

  f=fopen(nombre_fichero1,"rb");
  if(f == NULO){
    printf("\nNo se puede abrir el fichero %s\n",nombre_fichero1);
    return(-1);
  }

  // Obtiene el tamaño del fichero de datos señal
  fseek (f , 0 , SEEK_END);
  lon = ftell (f);

  rewind (f);
  
  switch(senal->tipo_muestra){
  
    case BITS_16_PCM :

      // Se reajusta el parametro senal->longitud a tamaño short (2 bytes)
      // numero muestras de señal es: señal->longitud / numero bytes muestra
      lon /= sizeof(short);
      senal->longitud = lon;      
      
      switch(tipo){
      
        case RAW_INTEL:

          // Lectura fichero con la señal + ruido
          fread(senal->muestras,sizeof(short),senal->longitud,f);
          
        break;
        
        case RAW_MOTOROLA:

          // Lectura fichero con la señal + ruido girando orden de bytes
          for(i=0;i<senal->longitud;i++){
            // leemos 2 bytes (short) 
            fread(&tmp,sizeof(short),1,f);
            // giramos orden de bytes
            aux = (short) (((tmp & 0x00FF)<<8) | ((tmp & 0xFF00)>>8));
            // guardamos el short girado 
            *(senal->muestras + i) = aux;
            
          }
          
        break;
      
        default:
        
          printf("\ntipo introducido no valido (%d)\n",tipo);
          return(-1);
          
        break;
          
      }
            
    break;
  
  }
     
  fclose(f);
  
  return(OK);
  
}

int leer_fichero_senal_WAV(Senal *senal,char *nombre_fichero1){  
  
  FILE *f;
  int tmpi;
  short tmps;
  int err;
  char buffer[5];
  //fin de string
  buffer[4]='\0';
  
  f=fopen(nombre_fichero1,"rb");
  if(f == NULO){
    printf("\nNo se puede abrir el fichero %s\n",nombre_fichero1);
    return(ERROR2);
  }
  
// Leemos la cabecera "RIFF"

  // Leemos los caracteres "RIFF" en fichero  
  fread(buffer,4,1,f);
  err=strcmp(buffer,"RIFF");
  if(err != 0) return(-1);

  // Leemos el tamaño total del fichero en bytes; datos + cabeceras  
  fread(&tmpi,4,1,f);
                
  // Leemos los caracteres "WAVE" en fichero  
  fread(buffer,4,1,f);
  err=strcmp(buffer,"WAVE");
  if(err != 0) return(-1);
  
// Leemos la cabecera "FORMAT"

  // Leemos los caracteres "fmt " en fichero  
  fread(buffer,4,1,f);
  err=strcmp(buffer,"fmt ");
  if(err != 0) return(-1);
  
  // Leemos el 0x01 de "fmt "
  fread(&tmpi,4,1,f);
  
    
  // Leemos el "null"
  fread(&tmps,2,1,f);
        
  // Leemos el numero de canales
  fread(&tmps,2,1,f);
  senal->canales = (int)tmps;
  printf(" Numero de canales %d\n",senal->canales);
  
  if (tmps != 1){
    printf(" No se puede leer WAVs de mas de 1 canal\n");
    exit(-2);
  }
    
  // Leemos la frecuencia de muestreo 
  fread(&tmpi,4,1,f);
  senal->frec_muestreo = (int)tmpi;
  printf(" Frecuencia de muestreo %d\n",senal->frec_muestreo);
  
  // Leemos el numero de bytes por segundo  
  fread(&tmpi,4,1,f);
  
  // Leemos el numero de bytes por muestra  
  fread(&tmps,2,1,f);
  senal->tipo_muestra = (int)tmps;
  printf(" Numero de bytes por muestra %d\n",senal->tipo_muestra);
  
  // Leemos el numero de bits por muestra  
  fread(&tmps,2,1,f);
  
// Leemos la cabecera "DATA"

  // Leemos los caracteres "data" en fichero  
  fread(buffer,4,1,f);
  err=strcmp(buffer,"data");
  if(err != 0) return(-1);

  // Leemos el tamaño de los datos en formato pcm  
  fread(&tmpi,4,1,f);
  senal->longitud = (int)(tmpi / senal->tipo_muestra);
  printf(" Numero de muestras %d\n",senal->longitud);
  
// Leemos los datos  
  
  // escribe las muestras filtradas en el fichero
  fread(senal->muestras,senal->tipo_muestra,senal->longitud,f);
  
  fclose(f);

  return(OK);

} 

int escribir_fichero_senal_RAW(Senal *senal,char *nombre_fichero2,int tipo){  

  int i;
  short tmp,aux;
  FILE *f;
    
  f=fopen(nombre_fichero2,"wb");
  if(f == NULO){
    printf("\nNo se puede abrir el fichero %s\n",nombre_fichero2);
    return(-1);
  }

  switch(tipo){
      
    case RAW_INTEL:

      // escritura fichero con las muestras filtradas
      fwrite(senal->muestras_filtradas,sizeof(short),senal->longitud,f);
        
    break;
        
    case RAW_MOTOROLA:

      // Lectura fichero con la señal + ruido girando orden de bytes
      for(i=0;i<senal->longitud;i++){

        // obtenemos una muestra filtrada (short)
        aux = *(senal->muestras_filtradas + i);
        // giramos orden de bytes
        tmp = (short) (((aux & 0x00FF)<<8) | ((aux & 0xFF00)>>8));
        // escribimos 2 bytes (short) 
        fwrite(&tmp,sizeof(short),1,f);
            
      }
        
    break;
      
    default:
        
      printf("\ntipo introducido no valido (%d)\n",tipo);
      return(-1);
          
    break;
          
  }

  fclose(f);

  return(OK);

}

int escribir_fichero_senal_WAV(Senal *senal,char *nombre_fichero2){  
  
  FILE *f;
  int tmp;
  char buffer[4];
  
  f=fopen(nombre_fichero2,"wb");
  if(f == NULO){
    printf("\nNo se puede abrir el fichero %s\n",nombre_fichero2);
    return(-1);
  }
  
// Guardando la cabecera "RIFF"

  // Guardamos los caracteres "RIFF" en fichero  
  sprintf(buffer, "%c%c%c%c", 'R', 'I', 'F', 'F');
  fwrite(buffer,4,1,f);
  
  // Guardamos el tamaño total del fichero en bytes; datos + cabeceras  
  tmp = (senal->tipo_muestra * senal->longitud) + 36;
  fwrite(&tmp,sizeof(int),1,f);
              
  // Guardamos los caracteres "WAVE" en fichero  
  sprintf(buffer, "%c%c%c%c", 'W', 'A', 'V', 'E');
  fwrite(buffer,4,1,f);
  
// Guardando la cabecera "FORMAT"

  // Guardamos la los caracteres "fmt " en fichero  
  sprintf(buffer, "%c%c%c%c", 'f', 'm', 't', ' ');
  fwrite(buffer,4,1,f);

  // Guardamos la longitud de "fmt"
  tmp=0x10;
  fwrite(&tmp,sizeof(int),1,f);
  
  // Guardamos el "null"
  sprintf(buffer, "%c%c", 0x01, 0x00);
  fwrite(buffer,2,1,f);
  
  // Guardamos el numero de canales
  sprintf(buffer, "%c%c", senal->canales, 0x00);
  fwrite(buffer,2,1,f);
  
  // Guardamos la frecuencia de muestreo 
  tmp = senal->frec_muestreo;
  fwrite(&tmp,sizeof(int),1,f);
  
  // Guardamos el numero de bytes por segundo  
  tmp = senal->frec_muestreo * senal->tipo_muestra;
  fwrite(&tmp,sizeof(int),1,f);
  
  // Guardamos el numero de bytes por muestra  
  sprintf(buffer, "%c%c", senal->tipo_muestra, 0x00);
  fwrite(buffer,2,1,f);

  // Guardamos el numero de bits por muestra  
  sprintf(buffer, "%c%c", (8*senal->tipo_muestra), 0x00);
  fwrite(buffer,2,1,f);
  
// Guardando la cabecera "DATA"

  // Guardamos los caracteres "data" en fichero  
  sprintf(buffer, "%c%c%c%c", 'd', 'a', 't', 'a');
  fwrite(buffer,4,1,f);

  // Guardamos el tamaño de los datos en formato pcm  
  tmp = senal->tipo_muestra * senal->longitud;
  fwrite(&tmp,sizeof(int),1,f);
  
// Guardando los datos  
  
  // escribe las muestras filtradas en el fichero
  fwrite(senal->muestras_filtradas,sizeof(short),senal->longitud,f);

  fclose(f);

  return(OK);

}

int escribir_fichero_VAD(Senal *senal,char *nombre_fichero3){  
  
  FILE *f;
  int i;
  int lon = senal->longitud;
  char fichero[256];
  
  sprintf(fichero,"%s_VAD.m",nombre_fichero3);
  
  f=fopen(fichero,"w");
  if(f == NULO){
    printf("\nNo se puede abrir el fichero %s\n",nombre_fichero3);
    return(-1);
  }
  
  // escribimos cabecera para definir vector en matlab
  fprintf(f, "VAD = [");
  for(i=0;i<lon;i++){
    fprintf(f, " %d", *(senal->VAD + i));
  }
  // escribimos parte final cabecera matlab
  fprintf(f, " ];\n");

  // escribimos cabecera para definir vector en matlab
  fprintf(f, "OUT = [");
  for(i=0;i<lon;i++){
    fprintf(f, " %d", *(senal->muestras_filtradas + i));
  }
  // escribimos parte final cabecera matlab
  fprintf(f, " ];\n");

  fprintf(f, "x = 0:1:%d;figure;plot(x,VAD,x,OUT);title('señal filtrada + VAD');\n", (lon-1));

  // escribimos cabecera para definir vector en matlab
  fprintf(f, "ERR = [");
  for(i=0;i<lon;i++){
    fprintf(f, " %d", (*(senal->muestras + i)) - (*(senal->muestras_filtradas + i)));
  }
  // escribimos parte final cabecera matlab
  fprintf(f, " ];\n");

  fprintf(f, "x = 0:1:%d;figure;plot(x,ERR);title('señal error (ruido)');\n", (lon-1));

  fclose(f);

  return(OK);

}

int guardar_senal(Senal *senal, float *muestras_filtradas, int numero_trama, int vad){

//  int i,j,bucle,inicio;
  int i,lon_puntas;
  int lon = senal->longitud_trama; 
  int desplaza = senal->desplaza_trama * numero_trama;
  register int ponderacion;
  register float tmp,valor_ventana;

  // calculo del promedio a utilizar
  tmp = senal->longitud_trama;
  tmp /= 2*senal->desplaza_trama;
  ponderacion = (int)tmp;

  for(i=0;i<lon;i++){
    tmp = *(muestras_filtradas + i);
    tmp/= ponderacion;
    *(senal->muestras_filtradas + desplaza + i) += (short)tmp;
  }
  
  return(OK);
        
}  

