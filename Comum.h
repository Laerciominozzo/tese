/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Comum.h
 * Author: lminozzo
 *
 * Created on 26 de Janeiro de 2017, 12:15
 */

#ifndef COMUM_H
#define COMUM_H

#ifdef RESOLUCAO_MEDIA
    #define MEUFREENECT_RESOLUCAO FREENECT_RESOLUTION_MEDIUM
    #define IMAGEM_LARGURA 320
    #define IMAGEM_ALTURA 240
#else
/// NÃO FUNCIONOU: what():  Cannot set depth format: invalid mode
    #define MEUFREENECT_RESOLUCAO FREENECT_RESOLUTION_LOW
    #define IMAGEM_LARGURA 320
    #define IMAGEM_ALTURA 240

#endif    

#ifndef ALARME_TEMPO
#define ALARME_TEMPO 1
#endif

#ifdef DESCONSIDERAR_ULTIMO_BIT
#define MASCARA 0xFE
#define DESLOCAMENTO_COR 0x1
#elif DESCONSIDERAR_ULTIMOS_2_BITS
#define MASCARA 0xFC
#define DESLOCAMENTO_COR 0x2
#elif DESCONSIDERAR_ULTIMOS_3_BITS
#define MASCARA 0xF8
#define DESLOCAMENTO_COR 0x3
#elif DESCONSIDERAR_ULTIMOS_4_BITS
#define MASCARA 0xF0
#define DESLOCAMENTO_COR 0x4
#endif

#ifdef HSV
#define CONVERSAO_JANELA CV_HSV2BGR
#define CONVERSAO_KINECT CV_RGB2HSV
#else
#define CONVERSAO_JANELA CV_RGB2BGR
#endif

#ifndef QTD_AMOSTRAS_TEMPO
#define QTD_AMOSTRAS_TEMPO 30
#endif

#endif /* COMUM_H */

