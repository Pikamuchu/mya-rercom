/***************************************************************************
                          utilidades.c  -  description
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

#include "definiciones.h"
#include "funciones.h"

int fft(float *real,float *imag,float *muestras,int lon){

  int k,n,m,bit;
  int m_k,n_medios,m_k_n_medios;
  float w,w_k;
  float res_real[lon],res_imag[lon];
  register float y1,y2,z1,z2,tmp;
         
  // Realizamos el "bit reverse"   
  for (n=0, m=0; n<lon; n++){
    // consideramos que las señales son reales 
    *(res_real + m)=*(muestras + n);
    *(res_imag + m)=0;
    for (bit=lon/2; (m & bit)!=0; bit >>= 1){
      m ^= bit; 
    }
    m ^= bit;
  }
   
  // calculo de la FFT por el metodo de la "mariposa"
  for (n=2; n<=lon; n <<= 1){ 
    w=-PI2/n;
    n_medios = n;
    n_medios >>= 1;
    for (m=0; m<lon; m+=n){ 
      for (k=0; k<n_medios; k++){ 
        m_k = m + k;
        m_k_n_medios = m_k + n_medios;
        w_k = w * k;
        // y1, z1 contiene la parte real de la FFT
        // y2, z2 contiene la parte imaginaria de la FFT 
        y1=*(res_real + m_k);
        y2=*(res_imag + m_k);
        z1=(*(res_real + m_k_n_medios) * cos(w_k));
        z1-=(*(res_imag + m_k_n_medios) * sin(w_k));
        z2=(*(res_imag + m_k_n_medios) * cos(w_k));
        z2+=(*(res_real + m_k_n_medios) * sin(w_k));
        *(res_real + m_k) = (y1+z1); 
        *(res_real + m_k_n_medios) = (y1-z1); 
        *(res_imag + m_k) = (y2+z2); 
        *(res_imag + m_k_n_medios) = (y2-z2); 
        tmp = *(res_real + m_k); 
        *(real + m_k) = tmp;
        tmp = *(res_real + m_k_n_medios); 
        *(real + m_k_n_medios) = tmp;
        tmp = *(res_imag + m_k); 
        *(imag + m_k) = tmp;
        tmp = *(res_imag + m_k_n_medios); 
        *(imag + m_k_n_medios) = tmp;
      } 
    }
  }
  
  return(OK);
  
}

int ifft(float *muestras,float *real,float *imag,int lon){

  int k,n,m,bit;
  int m_k,n_medios,m_k_n_medios;
  float w,w_k;
  float res_real[lon],res_imag[lon];
  register float y1,y2,z1,z2,tmp;
       
  // Realizamos el "bit reverse"   
  for (n=0, m=0; n<lon; n++){
    // consideramos que las señales son reales 
    *(res_real + m)=*(real + n);
    *(res_imag + m)=*(imag + n);
    for (bit=lon/2; (m & bit)!=0; bit >>= 1){
      m ^= bit; 
    }
    m ^= bit;
  }
  
  // calculo de la IFFT por el metodo de la "mariposa"
  for (n=2; n<=lon; n <<= 1){ 
    w=PI2/n;
    n_medios = n;    
    n_medios >>= 1; 
    for (m=0; m<lon; m+=n){ 
      for (k=0; k<n_medios; k++){ 
        m_k = m + k;
        m_k_n_medios = m_k + n_medios;
        w_k = w * k;
        // y1, z1 contiene la parte real de la FFT
        // y2, z2 contiene la parte imaginaria de la FFT 
        y1=*(res_real + m_k);
        y2=*(res_imag + m_k);
        z1=(*(res_real + m_k_n_medios) * cos(w_k));
        z1-=(*(res_imag + m_k_n_medios) * sin(w_k));
        z2=(*(res_imag + m_k_n_medios) * cos(w_k));
        z2+=(*(res_real + m_k_n_medios) * sin(w_k));
        *(res_real + m_k) = (y1+z1); 
        *(res_real + m_k_n_medios) = (y1-z1); 
        *(res_imag + m_k) = (y2+z2); 
        *(res_imag + m_k_n_medios) = (y2-z2); 
        tmp = *(res_real + m_k);
        tmp /= lon; 
        *(muestras + m_k) = tmp; 
        tmp = *(res_real + m_k_n_medios);
        tmp /= lon; 
        *(muestras + m_k_n_medios) = tmp; 
        //tmp = *(res_imag + m_k); 
        //tmp /= lon;
        //*(imag + m_k) = tmp;
        //tmp = *(res_imag + m_k_n_medios);
        //tmp /= lon;
        //*(imag + m_k_n_medios) = tmp; 

      } 
    }
  }
  
  return(OK);
  
}

int estimar_espectro(float *real, float *imag, float *espectro, int lon){
  
  int i;
  register double tmp;
      
  // Calculo del periodograma
  // Px(w) = |X(w)|^2 / N
  
  for(i=0;i<lon;i++){
    tmp = ((pow(*(real + i),2) + pow(*(imag + i),2)));
    tmp /= lon;
    *(espectro + i) = (float)tmp;
  }
  
  return(OK);
  
}

int promediar_espectro(float *espectro, float *espectro_promed, float gamma, int lon){
  
  int i;
  register float tmp;
  
  // Realizamos la operacion:
  // P(w,n) = gamma*P(w,n) + (1-gamma)*P(w,n-1)
  
  if(gamma <= 1){ 
    
    for(i=0;i<lon;i++){
      tmp = (gamma * (*(espectro + i))) + ((1-gamma) * (*(espectro_promed + i)));
      *(espectro_promed + i) = tmp;
    }
  
  }
  
  return(OK);
  
}

int correlacion(float *muestras, float *correlaciones, int long_trama, int orden){

  int i,n;
  register float tmp;
    
/************************** Calculo correlación *******************************/
      
  // Estimacion correlacion sesgada
  // Rx(i)= SUM x(n)x(n+i) / N
      
  // *(correlaciones)          ->    R(0)
  // *(correlaciones + orden)  ->    R(orden)
         
  for(i=0;i<=orden;i++){
    tmp = 0;
    for(n=0;n<(long_trama-i);n++){
      tmp += (*(muestras + n))*(*(muestras + n + i));
    }
    tmp /= long_trama;
    *(correlaciones + i) = tmp;
    // printf("\nR(%d) = %f",i,*(correlaciones + i));
  }
  
  return(OK);

}

int obtener_ak_levinson_durvin(float *aks, float *correlaciones, int orden){

  int i,k;
  register float p,tmp,tmp1,tmp2;

  // Inicialización parametro P a R[0] 
  p = *(correlaciones); 
  // El primer coeficiente ak debe ser 1 
  *(aks) = 1.0;

  // Para cada coeficiente ak
  for(k=1;k<=orden;k++){

    // Encontramos el siguiente coeficiente a partir de los ak anteriores 
    // y las autocorrelaciones R[.]
    tmp = 0;

    for(i=1;i<=k;i++){
      tmp -= (*(aks + (k-i)))*(*(correlaciones + i));
    }

    *(aks + k) = tmp/p;

    // realizamos la recursion de los ak
    for(i=1;i<=k/2;i++){
          
      tmp1 = (*(aks + i)) + (*(aks + k)) * (*(aks + (k-i)));
      tmp2 = (*(aks + (k-i))) + (*(aks + k)) * (*(aks + i));
          
      *(aks + i) = tmp1;
      *(aks + (k-i)) = tmp2;
    }

    // Calculamos el nuevo parametro P
    p = p * (1.0 - (*(aks + k))*(*(aks + k)));

    if (p <= 0) {
      // Se ponen el resto de parametros ak a cero 
      k++;
      for(;k<=orden;k++){
        *(aks + k) = 0;
      }
    }

  }
  

  return(0);
  
}

float estimar_pitch(float *muestras, int longitud_trama, float sobreumbral){

  //int k,lon;
  int i,inicio,err;
  float pitch_tiempo,pitch_frecuencia;
  float corre[longitud_trama];
  register float tmp,max,min,umbral;

  err=correlacion(muestras,corre,longitud_trama,((longitud_trama/2)-1));
    
    
  max=0;
  min=0;
  inicio=0;
  // busqueda del primer mínimo (mínimo mayor)
  for(i=0;i<longitud_trama/2;i++){
    tmp = *(corre + i);
    if(tmp<min){
      min = tmp;
      inicio = i;
    }
  }
  // busqueda del segundo maximo (el primero es R(0))
  // se busca a partir del primer mínimo
  for(i=inicio;i<longitud_trama/2;i++){
    tmp = *(corre + i);
    if(tmp>max){
      max = tmp;
      pitch_tiempo = i;
    }
  }
  
  // conversión pitch tiempo a pitch frecuencia

  if(pitch_tiempo < MAX_MUESTRAS_PITCH){
    
    if(pitch_tiempo > MIN_MUESTRAS_PITCH){
      
      // calculando umbral de pitch valido
      // umbral = (sobreumbral*R(0)*(longitud_trama-pitch_tiempo)/longitud_trama
      umbral = *(corre);
      umbral *= (longitud_trama - pitch_tiempo);
      umbral *= sobreumbral;
      umbral /= longitud_trama;
      
      
      if(max > umbral){
        //conversion pitch_frecuencia = longitud_trama/pitch_tiempo
        pitch_frecuencia = longitud_trama;
        pitch_frecuencia /= pitch_tiempo;
        // printf("\npitch_frecuencia es %f",pitch_frecuencia);
      }else{
        // pitch no valido
        pitch_frecuencia = 0;
      }  
      
    }else{
      // pitch no valido
      pitch_frecuencia = 0;
    }
       
  }else{
    
    // pitch no valido
    pitch_frecuencia = 0;   
  
  }
  
  return(pitch_frecuencia);
  
}

float distancia_espectral(float *espectro_ruido,float pot_ruido, float *espectro_senal, float pot_senal, int lon){

  int k=0;
  register float log_ruido,log_senal,res,tmp;
  
  res = 0;
  for(k=0;k<lon;k++){
    // parte del ruido
    tmp = *(espectro_ruido + k);
    tmp = log(tmp);
    // evitamos que log_ruido de error o negativo
    if(tmp<1){
      log_ruido = 0;
    }else{
      log_ruido = log(tmp);
    }
    // parte de la senal
    tmp = *(espectro_senal + k);
    // ajuste de potencia espectro senal
    tmp *= pot_ruido;
    tmp /= pot_senal;
    tmp = log(tmp);    
    // evitamos que log_senal de error o negativo
    if(tmp<1){
      log_senal = 0;
    }else{
      log_senal = log(tmp);
    }
    // calculo distancia
    tmp = log_ruido - log_senal;

    if(tmp < 0){
      res -= tmp;
    }else{
      res += tmp;
    }
  }
  
  return(res);
}

int enventanar(float *muestras_out, float *muestras_in, int longitud_trama){

  int i;
  register float tmp;
  
  for(i=0;i<longitud_trama;i++){

     tmp = (*(muestras_in + i));
     tmp *= 0.5*(1 - cos((PI2*i)/(longitud_trama-1)));
    *(muestras_out + i) = tmp;
  
  }

  return(OK);
  
}

float potencia_espectro(float *espectro, int lon){

  int i = 0;  
  register float potencia = 0;
            
  for(i=0;i<lon;i++){  
    // Calculo potencia espectro
    potencia += *(espectro + i);
  }

  potencia /= lon;
  
  return(potencia);
  
}

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
                       float delta){

  int k;
  float p;
  float peine[lon];
  register float tmp,tmp1,mod;

  if(pitch>0){
    //construccion filtro peine
    //inicialización
    for(k=0;k<lon;k++){
      *(peine + k) = 0.5;
    }
    // creación puas
    for(p=pitch;p<(lon-1);p+=pitch){
      k=(int)(p+0.5);
      *(peine+(k-1)) = 1;
      *(peine+k) = 1;
      *(peine+(k+1)) = 1;
    }
  }
    
  // eliminacion DC
  *(dft_modulo_filtro) = atenuacion;

  // filtro paso alto -> elimina componentes baja frecuencia
  for(k=1;k<=corte_bajo;k++){
    *(dft_modulo_filtro + k) = atenuacion;
    *(dft_modulo_filtro + lon - k) = atenuacion;
  }
  
  // filtro paso bajo -> elimina componentes alta frecuencia
  for(k=corte_alto;k<=(lon-corte_alto);k++){
    *(dft_modulo_filtro + k) = atenuacion;
  }
      
  switch(tipo_filtro){

    case WIENER_1:
  
      // H(w) = ( Ps(w) / (Ps(w) + beta * Pd(w)) )^delta
      
      // primera mitad espectro
      for(k=(corte_bajo+1);k<corte_alto;k++){
        
        // Calculo del modulo del filtro
        tmp = *(espectro_modelado + k);
        tmp /= (tmp + (beta * (*(espectro_ruido + k))));

        // para evitar problemas de modulos negativos
        if(tmp <= 0){
          mod = 0;
        }else{
          mod = (float)pow(tmp,delta);
        }
          
        if(pitch>0){
          mod *= *(peine + k);
        } 
                                                
        // Consideramos que el filtro atenua un maximo de aten
        if( mod < atenuacion){
          *(dft_modulo_filtro + k) = atenuacion;
        }else{
          *(dft_modulo_filtro + k) = mod;
        }
    
      }
        
      // segunda mitad espectro
      for(k=(lon-corte_alto+1);k<(lon-corte_bajo);k++){
        
        // Calculo del modulo del filtro
        tmp = *(espectro_modelado + k);
        tmp /= (tmp + (beta * (*(espectro_ruido + k))));

        // para evitar problemas de modulos negativos
        if(tmp <= 0){
          mod = 0;
        }else{
          mod = (float)pow(tmp,delta);
        }

        if(pitch>0){
          mod *= *(peine + k);
        } 
                          
        // Consideramos que el filtro atenua un maximo de aten
        if( mod < atenuacion){
          *(dft_modulo_filtro + k) = atenuacion;
        }else{
          *(dft_modulo_filtro + k) = mod;
        }
    
      }
      
    break;

    case WIENER_2:
    
      // H(w) = ((Px(w) - beta*Pd(w)) / Px(w))^delta

      // primera mitad espectro
      for(k=(corte_bajo+1);k<corte_alto;k++){        
        // Calculo del modulo del filtro
        tmp1 = *(espectro_voz_ruido + k);
        tmp = (tmp1 - (beta * (*(espectro_ruido + k))));
        tmp /= tmp1;

        // para evitar problemas de modulos negativos
        if(tmp <= 0){
          mod = 0;
        }else{
          mod = (float)pow(tmp,delta);
        }
          
        if(pitch>0){
          mod *= *(peine + k);
        }
                                          
        // Consideramos que el filtro atenua un maximo de aten
        if( mod < atenuacion){
          *(dft_modulo_filtro + k) = atenuacion;
        }else{
          *(dft_modulo_filtro + k) = mod;
        }
    
      }

      // segunda mitad espectro
      for(k=(lon-corte_alto+1);k<(lon-corte_bajo);k++){        
        // Calculo del modulo del filtro
        tmp1 = *(espectro_voz_ruido + k);
        tmp = (tmp1 - (beta * (*(espectro_ruido + k))));
        tmp /= tmp1;
          
        // para evitar problemas de modulos negativos
        if(tmp <= 0){
          mod = 0;
        }else{
          mod = (float)pow(tmp,delta);
        }
          
        if(pitch>0){
          mod *= *(peine + k);
        }
                                                          
        // Consideramos que el filtro atenua un maximo de aten
        if( mod < atenuacion){
          *(dft_modulo_filtro + k) = atenuacion;
        }else{
          *(dft_modulo_filtro + k) = mod;
        }
    
      }

    break;

  }
  
  return(OK);

}

int filtrar(float *dft_real,
            float *dft_imag,
            float *dft_modulo_filtro,
            int lon){

  int k;
  register float tmp,tmp_r,tmp_i;
  
  // filtrado de la señal enventanada   
  for(k=0;k<lon;k++){
  
    tmp = *(dft_modulo_filtro + k);
    
    // parte real del filtrado 
    tmp_r = *(dft_real + k);
    tmp_r *= tmp;

    // parte imaginaria del filtrado
    tmp_i = *(dft_imag + k);
    tmp_i *= tmp;

    *(dft_real + k) = tmp_r;
    *(dft_imag + k) = tmp_i;
 
  }
 
  return(OK);

}

int atenuar(float *dft_real,
            float *dft_imag,
            float aten,
            int lon){

  int k;
  register float tmp_r,tmp_i;
  
  // filtrado de la señal enventanada   
  for(k=0;k<lon;k++){

    // parte real del filtrado 
    tmp_r = *(dft_real + k);
    tmp_r *= aten;

    // parte imaginaria del filtrado
    tmp_i = *(dft_imag + k);
    tmp_i *= aten;

    *(dft_real + k) = tmp_r;
    *(dft_imag + k) = tmp_i;
 
  }
 
  return(OK);

}

int espectro_modelado(float *espectro,
                      float *aks,
                      float pot_senal,
                      float pot_ruido,
                      int long_trama,
                      int orden){

  int i,k;
  float pot_ar,g;
  register float tmp,tmp1,tmp2,w1,w2,w3;

/****** Calculo del coeficiente g para estimacion espectro señal limpia *******/
      
  // Calculo espectro de potencia modelado AR considerando g=1
  w1=-PI2/long_trama;
   
  for(i=0;i<long_trama;i++){
    // parte real
    tmp1=1;
    // parte imaginaria
    tmp2=0;
    // lo de la exponencial
    w2=w1*i;
    for(k=1;k<=orden;k++){
      w3=w2*k;
      tmp1 += (*(aks + k))*cos(w2*k);
      tmp2 += (*(aks + k))*sin(w2*k);
    }
    tmp = pow(tmp1,2) + pow(tmp2,2);
    *(espectro + i) = 1/tmp;
  }

  // calculo potencia espectro modelado considerando g = 1
  pot_ar = potencia_espectro(espectro, long_trama);
      
  // Caso en que al potencia de señal es superior a la de ruido
  if (pot_senal > pot_ruido){
  
    g = (pot_senal-pot_ruido)/pot_ar;

  // Caso en que la pot de señal es inferior a la de ruido 
  }else{
    
    g = 0;
  
  }
          
/******************************************************************************/

/************* Calculo espectro del modelo AR de la senyal ********************/      

  for(i=0;i<long_trama;i++){  
    *(espectro + i) *= g;
  }

/******************************************************************************/

  return(OK); 
  
}

