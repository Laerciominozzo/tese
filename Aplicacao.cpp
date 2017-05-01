/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Aplicacao.cpp
 * Author: lminozzo
 * 
 * Created on 25 de Janeiro de 2017, 21:41
 */

#include "Aplicacao.h"
#include"ImgParalela.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <signal.h>
#include <unistd.h>
#include "ImgSequencial.h"



Aplicacao::Aplicacao() {
    
    meuKinect=&freenect.createDevice<MeuKinect>(0);
#ifdef EPIPHANY
    img=new ImgParalela();
#else
    img=new ImgSequencial();
#endif
    
    meuKinect->inicia();
#ifdef MOSTRAR_VIDEO
   cv::namedWindow("rgb",CV_WINDOW_AUTOSIZE);cv::namedWindow("bin",CV_WINDOW_AUTOSIZE);
#endif
}

void Aplicacao::corre() {
   
   cv::Mat auxRGB(IMAGEM_ALTURA, IMAGEM_LARGURA, CV_16UC1);
   cv::Mat auxProf(IMAGEM_ALTURA, IMAGEM_LARGURA, CV_16UC1);
   
   
   int cont=0;
   int retorno;
    do{// loop principal da aplicação
       // temporizador.marcarInicio("Loop");
    //    temporizador.marcarInicio("CopiarRGB");
        if(!(retorno=meuKinect->getRGBCompactado(auxRGB))) //se teve que esperar
            qtdRGBNaoRetornadas++;              //nao conta não retornadas
        else
            qtdRGBPerdidas+=(retorno-1);// se não teve que esperar, soma a quadotade perdida menos a atual
    //    tempos.setTempo("CopiarRGB",temporizador.marcarFim("CopiarRGB"));
        
   //     temporizador.marcarInicio("CopiarProf");
        if(!(retorno=meuKinect->getProf(auxProf)))
            qtdProfNaoRetornadas++;
        else    
            qtdProfPerdidas+=(retorno-1); 
    //    tempos.setTempo("CopiarProf",temporizador.marcarFim("CopiarProf"));-D MO-D MOSTRAR_VIDEOSTRAR_VIDEO
        
        img->setRGB(auxRGB);
        img->setProf(auxProf);
        
        temporizador.marcarInicio("Binarizar");
        img->binariza();
        tempos.setTempo("Binarizar",temporizador.marcarFim("Binarizar"));
        
      //  temporizador.marcarInicio("Morf.Matematica");
       // img->filtrarRuido();         
        /*
         O filtra ruido foi tirado por não ser possivel jogar as funções do OpenCV no coprocessador.
         */
     //   tempos.setTempo("Morf.Matematica",temporizador.marcarFim("Morf.Matematica"));
        
        temporizador.marcarInicio("Momento");
        if(!(bolaEncontrada=img->getCentro(&posBolaX,&posBolaY))){
            qtdBolaNaoEncontrada++;
            profBola=0;
        }else
            profBola=img->getProf(posBolaX,posBolaY);
        
        tempos.setTempo("Momento",temporizador.marcarFim("Momento"));
        qtdTotal++;
        
        
        
#ifdef MOSTRAR_VIDEO
        temporizador.marcarInicio("Video");
        mostrarImagens((char*)"rgb",(char*)"bin");
        cv::waitKey(1);
        tempos.setTempo("Video",temporizador.marcarFim("Video"));
#endif
      //  tempos.setTempo("Loop",temporizador.marcarFim("Loop"));
        if(++cont>500)kill(getpid(),SIGINT);
    }while(continua);

}

void Aplicacao::getCoresBola() {
    
    cv::Mat imagem(IMAGEM_ALTURA, IMAGEM_LARGURA, CV_8UC3);
    JanelaCalibracao* janela=new JanelaCalibracao(img,Img::adicionaCorBola);
    
    continua=false;
    std::cout<<"Retirando amostras de cores. Precione \'n\' ou \'N\' para nova recolha ou qualquer tecla para finalizar.\n";
    do{
        meuKinect->getRGB(imagem);
        corre();  
        std::cout<<"Quantidade de cores recolhidas:"<<img->getQtdAmostras()<<std::endl;
    }while(janela->mostrar(imagem));    //enquanto vai apertando 'n' abrir novamente a imagem
    std::cout<<"Quantidade de cores recolhidas:"<<img->getQtdAmostras()<<std::endl;  
    
    delete janela;
}

std::string Aplicacao::verDados() {
    std::string retorno="\n";
    char numero[50];
   // retorno+=meuKinect->verTempos();
    retorno+="\n";
    retorno+=tempos.verDados();
    sprintf(numero,"%lu",qtdTotal);
    retorno+=(std::string)"\n\n  Quantidade de imagens tiradas:........................................."+numero;
    sprintf(numero,"%lu",qtdRGBPerdidas);
    retorno+=(std::string)"\n\n  Quantidade de imagens RGB perdidas:...................................."+numero;
    sprintf(numero,"%lu",qtdProfPerdidas);
    retorno+=(std::string)"\n  Quenatidade de Imagens de profundidade perdidas:......................."+numero;
    sprintf(numero,"%lu",qtdRGBNaoRetornadas);
    retorno+=(std::string)"\n  Quantidade de vezes que o kinect não tinha a imagem RGB:..............."+numero;
    sprintf(numero,"%lu",qtdRGBNaoRetornadas);
    retorno+=(std::string)"\n  Quantidade de vezes que o kinect não tinha a imagem de profundidade:..."+numero;
    sprintf(numero,"%lu",qtdBolaNaoEncontrada);
    retorno+=(std::string)"\n\n  Quantidade de vezes em que a bola não foi encontrada:.................."+numero;
    sprintf(numero,"%d",posBolaX);
    retorno+=(std::string)"\n\n  Posicao X:............................................................."+numero;
    sprintf(numero,"%d",posBolaY);
    retorno+=(std::string)"\n  Posicao Y:............................................................."+numero;
    sprintf(numero,"%d",profBola);
    retorno+=(std::string)"\n\n  Distancia da câmera:..............................................."+numero;
    retorno+="\n";
    
    return retorno;
}

void Aplicacao::mostrarImagens(char* nomeJanelaRGB, char* nomeJanelaBin) {
    cv::Mat imgBGR;//(IMAGEM_ALTURA, IMAGEM_LARGURA, CV_8UC3);
  //  meuKinect->getRGB(imgBGR);
    imgBGR=img->getRGB();
  //  cv::cvtColor(imgBGR,imgBGR,CONVERSAO_JANELA);      //colocar define para hsv
    cv::Scalar cor(0,255,0);
    if(!bolaEncontrada)
         cor=cv::Scalar(0,0,255);;
    
                                                                                //desenha um alvinho na posicao da bola
    	cv::circle(imgBGR,cv::Point(posBolaX,posBolaY),20,cor,2);
    if(posBolaY-25>0)
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(posBolaX,posBolaY-25),cor,2);
    else 
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(posBolaX,0),cor,2);
    if(posBolaY+25<imgBGR.rows)
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(posBolaX,posBolaY+25),cor,2);
    else 
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(posBolaX,imgBGR.rows),cor,2);
    if(posBolaX-25>0)
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(posBolaX-25,posBolaY),cor,2);
    else 
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(0,posBolaY),cor,2);
    if(posBolaX+25<imgBGR.cols)
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(posBolaX+25,posBolaY),cor,2);
    else 
        cv::line(imgBGR,cv::Point(posBolaX,posBolaY),cv::Point(imgBGR.cols,posBolaY),cor,2);
        
    cv::imshow(nomeJanelaBin,img->getBin());
    cv::imshow(nomeJanelaRGB,imgBGR);
   
}

void Aplicacao::zerarDados() {
    qtdBolaNaoEncontrada=0;
    qtdProfNaoRetornadas=0;
    qtdProfPerdidas=0;
    qtdRGBNaoRetornadas=0;
    qtdRGBPerdidas=0;
    qtdTotal=0;
    meuKinect->zerarDados();
}


Aplicacao::~Aplicacao() {
    meuKinect->finaliza();
    freenect.deleteDevice(0);
    delete img;
#ifdef MOSTRAR_VIDEO
    cv::destroyWindow("rgb");cv::destroyWindow("bin");
#endif
}
