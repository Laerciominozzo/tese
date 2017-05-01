/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeuKinect.cpp
 * Author: lminozzo
 * 
 * Created on 25 de Janeiro de 2017, 21:40
 */

#include "MeuKinect.h"
#include "Comum.h"
#include <time.h>
#include <unistd.h>
 

MeuKinect::MeuKinect(freenect_context* ctx, int index):
Freenect::FreenectDevice(ctx,index),
imgRGB(IMAGEM_ALTURA,IMAGEM_LARGURA,CV_8UC3),
imgProf(IMAGEM_ALTURA,IMAGEM_LARGURA,CV_16UC1){
        setDepthFormat(FREENECT_DEPTH_11BIT,MEUFREENECT_RESOLUCAO);  
        /*Bits não compactados para facilitar o desenvolvimento e ganhar em processamento*/
        setVideoFormat(FREENECT_VIDEO_RGB,MEUFREENECT_RESOLUCAO);
        
}

void MeuKinect::inicia() {
    startVideo();
    startDepth();
}

void MeuKinect::finaliza() {
    stopDepth();
    stopVideo();
}


void MeuKinect::DepthCallback(void* _depth, uint32_t timestamp) {
  //  tempos.setTempo("LoopProf",temporizador.marcarFim("LoopProf"));
 //   temporizador.marcarInicio("CallBackProf");
    std::unique_lock<std::mutex> lck(mutexProf,std::defer_lock);
    if(lck.try_lock()){
  //      temporizador.marcarInicio("CopiarProf");
        uint8_t* imagem=(uint8_t*)_depth;               //<<-- o dado vem com 16 bits de resolução pra cada pixel
        int nLinhas=imgProf.rows;  //determina as linhas 
        int nColunas=imgProf.cols*imgProf.channels();//determina as colunas sendo que chanel é 1
        
        for(int j=0;j<nLinhas;j++){
            uint8_t* linha=imgProf.ptr<uint8_t>(j); //pega a linha
            for(int i=0;i<nColunas;i++){//x
                uint16_t som=(uint16_t) imagem[j*2*nColunas+i]+imagem[j*2*nColunas+i+1]+
                              imagem[j*2*nColunas+i+IMAGEM_LARGURA]+imagem[j*2*nColunas+i+1+IMAGEM_LARGURA];
                linha[i]=(uint8_t)(som/4);
            }
        }
        quadrosPerdidosProf++;          //copiar os dados aqui 
   //     tempos.setTempo("CopiarProf",temporizador.marcarFim("CopiarProf"));
        cond.notify_one();
    }
  //  tempos.setTempo("CallBackProf",temporizador.marcarFim("CallBackProf"));    
}

void MeuKinect::VideoCallback(void* _rgb, uint32_t timestamp) { 
   // tempos.setTempo("LoopRGB",temporizador.marcarFim("LoopRGB"));
  //  temporizador.marcarInicio("CallBackRGB");
    std::unique_lock<std::mutex> lck(mutexRGB,std::defer_lock);
    if(lck.try_lock()){
     //   temporizador.marcarInicio("CopiarRGB");
        uint8_t *imagem=(uint8_t*)_rgb;
        int nLinhas=imgRGB.rows;  //determina as linhas 
        int nColunas=imgRGB.cols;//determina as colunas sendo que chanel é 1
        
        for(int j=0;j<nLinhas;j++){//y
            uint8_t *linha=imgRGB.ptr<uint8_t>(j);
            for(int i=0;i<nColunas;i++){//x
                 for(int k=0;k<3;k++){                           //usar dois FORs ou o esquema dos steps para fazer a media
                    uint16_t som= (uint16_t)imagem[j*nColunas*6+i*6+k]+imagem[j*nColunas*6+i*6+k+3]+
                                            imagem[(j+1)*nColunas*6+i*6+k]+imagem[(j+1)*nColunas*6+i*6+k+3];
                    imgRGB.data[j*nColunas*3+i*3+k]=(uint8_t)(som/4);
                 }
            }
        }
        quadrosPerdidosRGB++;
      // tempos.setTempo("CopiarRGB",temporizador.marcarFim("CopiarRGB"));
        cond.notify_one();
    } 
 //   tempos.setTempo("CallBackRGB",temporizador.marcarFim("CallBackRGB")); 
}

int MeuKinect::getProf(cv::Mat& img) {
    int a;
    std::unique_lock<std::mutex> lck(mutexProf);
    a=quadrosPerdidosProf;      //copia a quantidade de imagens tiradas até agora para o retorno
    
    while(!quadrosPerdidosProf)  //enquanto não tirou imagens
        cond.wait(lck);    //dorme
    
    quadrosPerdidosProf=0;      //zera o contador 
    int nLinhas=imgProf.rows;  //determina as linhas 
    int nColunas=imgProf.cols*imgProf.channels()/4;//de(IMAGEM_ALTURA,IMAGEM_LARGURA,CV_16UC1)termina as colunas sendo que chanel é 1

    for(int j=0;j<nLinhas;j++){
        uint64_t* linhaB=imgProf.ptr<uint64_t>(j); //pega a linha
        uint64_t* linhaS=img.ptr<uint64_t>(j); //pega a linha
        for(int i=0;i<nColunas;i++)//x
            linhaS[i]=linhaB[i];
    }// copia a imagem
    return a;                   //retorna
}

int MeuKinect::getRGB(cv::Mat& img) {
    int a;
    std::unique_lock<std::mutex> lck(mutexRGB);
    a=quadrosPerdidosRGB;
    
    while(!quadrosPerdidosRGB)
        cond.wait(lck);
    
#ifdef CONVERSAO_KINECT
    cv::cvtColor(imgRGB,img,CONVERSAO_KINECT);
#else
    int nLinhas=imgRGB.rows;  //determina as linhas 
    int nColunas=imgRGB.cols*imgRGB.channels()/8;//determina as colunas sendo que chanel é 1
        
        for(int j=0;j<nLinhas;j++){//y
            uint64_t *linhaB=imgRGB.ptr<uint64_t>(j);
            uint64_t *linhaS=img.ptr<uint64_t>(j);
            for(int i=0;i<nColunas;i++)//x
                linhaS[i]=linhaB[i];
        }
#endif
    
    quadrosPerdidosRGB=0;
    return a;
}

int MeuKinect::getRGBCompactado(cv::Mat& img) {
    int a;
    std::unique_lock<std::mutex> lck(mutexRGB);
    a=quadrosPerdidosRGB;
    
    while(!quadrosPerdidosRGB)
        cond.wait(lck);
    //feito o acesso unidimencional por questão de facilidade no raciocinio.
    int nLinhas=imgRGB.rows;  //determina as linhas 
    int nColunas=imgRGB.cols;//determina as colunas sendo que chanel é 1
        
    for(int j=0;j<nLinhas;j++){//y
        uint8_t *linhaB=imgRGB.ptr<uint8_t>(j);
        uint16_t *linhaS=img.ptr<uint16_t>(j);
        for(int i=0;i<nColunas;i++)//x
            linhaS[i]=((uint16_t)(linhaB[i*3] & 0xF8)<<0x8)|((uint16_t)(linhaB[i*3+1] & 0xFC)<<0x3)|(linhaB[i*3+2]>>3);
    }
    quadrosPerdidosRGB=0;
    return a;
}


void MeuKinect::zerarDados() {
    
    std::lock (mutexProf,mutexRGB);
    quadrosPerdidosProf=0;
    quadrosPerdidosRGB=0;
    mutexProf.unlock();
    mutexRGB.unlock();
    
}

std::string MeuKinect::verTempos() {
    std::lock (mutexProf,mutexRGB);
    std::string ret=tempos.verDados();
    mutexProf.unlock();
    mutexRGB.unlock();
    return ret;
    
}


MeuKinect::~MeuKinect() {
    
    imgProf.release();
    imgRGB.release();
}





