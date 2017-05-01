/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JanelaCalibracao.cpp
 * Author: lminozzo
 * 
 * Created on 25 de Janeiro de 2017, 21:42
 */  

#include "JanelaCalibracao.h"
#include "Comum.h"
#include "Img.h"

 JanelaCalibracao::JanelaCalibracao(void* obj, void(*func)(void*, unsigned char, unsigned char, unsigned char)) {
    this->obj=obj;
    this->func=func;
    cv::namedWindow("Pegar Amostras de Cores",cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Pegar Amostras de Cores",mouseCallBack,this);
    selecionando=false;
}

void JanelaCalibracao::mouseCallBack(int evento, int x, int y, int, void* This) {
    JanelaCalibracao* obj=(JanelaCalibracao*)This;
    cv::Mat imagem;
    cv::cvtColor(obj->img,imagem,CONVERSAO_JANELA);
    switch( evento){
        case cv::EVENT_LBUTTONDOWN:
            obj->selX=x; 
            obj->selY=y;
            obj->selecionando=true;
            break;
        case cv::EVENT_MOUSEMOVE:
            if(obj->selecionando){
               
                int *xMenor,*yMenor,*xMaior,*yMaior;
                if(x<obj->selX){
                    xMenor=&x;
                    xMaior=&obj->selX;
                }else{
                    xMaior=&x;
                    xMenor=&obj->selX;
                }
                if(y<obj->selY){
                    yMenor=&y;
                    yMaior=&obj->selY;
                }else{
                    yMaior=&y;
                    yMenor=&obj->selY;
                }
                for(int i=*xMenor;i<*xMaior;i++)
                    for(int j=*yMenor;j<*yMaior;j++)
                        imagem.at<cv::Vec3b>(j,i)[0]+=100;
                cv::imshow("Pegar Amostras de Cores",imagem); 
            }
            break;
        case cv::EVENT_LBUTTONUP:
            if(!obj->selecionando)break;
            int *xMenor,*yMenor,*xMaior,*yMaior;
            if(x<obj->selX){
                xMenor=&x;
                xMaior=&obj->selX;
            }else{
                xMaior=&x;
                xMenor=&obj->selX;
            }
            if(y<obj->selY){
                yMenor=&y;
                yMaior=&obj->selY;
            }else{
                yMaior=&y;
                yMenor=&obj->selY;
            }
            for(int i=*xMenor;i<*xMaior;i++)
                for(int j=*yMenor;j<*yMaior;j++)
                    obj->func(obj->obj,obj->img.at<cv::Vec3b>(j,i)[0],obj->img.at<cv::Vec3b>(j,i)[1],obj->img.at<cv::Vec3b>(j,i)[2]);
            obj->selecionando=false;
            
            cv::imshow("Pegar Amostras de Cores",imagem);
            break;
        case cv::EVENT_RBUTTONDOWN:
            obj->selecionando=false;
            std::cout<<"\nSeleção cancelada\n";
            cv::imshow("Pegar Amostras de Cores",imagem);
            break;
    }
     
}

bool JanelaCalibracao::mostrar(cv::Mat img) {
    this->img=img.clone();         //coloca a imagem para ser acessada pela função de callbac
    cv::cvtColor(img,img,CONVERSAO_JANELA);   //converte para BGR para aparecer bonitinho na janela
    
    char tecla;                     //tecla do usuario, N ou n para nova coleta
    cv::imshow("Pegar Amostras de Cores",img);  //mostra a imagem na tela
    tecla=cv::waitKey();                        // espera pela resposta 
    
    return tecla=='n'||tecla=='N';
}


JanelaCalibracao::~JanelaCalibracao() {
    cv::destroyWindow("Pegar Amostras de Cores");
    img.release();
}


