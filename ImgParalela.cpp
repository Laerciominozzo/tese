/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImgSequencial.cpp
 * Author: lminozzo
 * 
 * Created on 25 de Janeiro de 2017, 21:45
 */



#include "ImgParalela.h"

#include "Mutex.h"
#include "Mensagem.h"
//#include <omp.h>
#include <e-hal.h>
#include <e-loader.h>
ImgParalela::ImgParalela(){
   // int lim= 0x2000;   //coloca limite maximo

    for(int i=0;i<0x2000;i++)
        coresBola[i]=0;
        qtdAmostras=0;
    imgBin=cv::Mat::zeros(IMAGEM_ALTURA,IMAGEM_LARGURA,CV_8UC1);
    
    e_init(NULL);
    e_reset_system();
    
    e_alloc(&emem,BUF_OFFSET,DESLOCAMENTO_DADOS+IMAGEM_ALTURA*IMAGEM_LARGURA*3);  //<<--RGB e Bin necessitam estar em zonas diferentes
     //tamanho = Mensagem + Imagem 16bits + imagem 8bits.
    e_open(&edev, 0, 0, 4, 4);
    if (e_load_group((char*)"e_aplicacao", &edev, 0, 0, 4,4, E_FALSE) == E_ERR)
    {
      printf("e_load failed");
    }
    e_start_group(&edev);
}


cv::Mat ImgParalela::getBin() {           //<<--tirei a clonagem da imagem para ficar mais rapido
    return imgBin;                          //vendo que para apresentar a imagem não vai ser concorrente
}

void ImgParalela::setRGB(cv::Mat img) {
    imgRGB=img;
     
}

void ImgParalela::eProcessa(uint8_t op, void* dados, uint32_t tamanho) {
    Mensagem msg;
    for(int i=0;i<NCORES;i++)
	msg.finalizou[i]=0;
    msg.operacao=op;
//printf("%x\t%x\n",tamanho,BUF_OFFSET+sizeof(msg)+tamanho);
    e_write(&emem,0,0,0,&msg,sizeof(Mensagem));
    if(tamanho>0)
        e_write(&emem,0,0,(off_t)DESLOCAMENTO_DADOS,dados,tamanho);
    
    int row, col;
    for (row = 0; row < 4; row++)
      for (col = 0; col < 4; col++)
        e_resume(&edev, row, col);
}

bool ImgParalela::eFinalizou() {
    Mensagem msg;
    e_read(&emem,0,0,0,&msg,sizeof(Mensagem));
    
    uint8_t som=0;
    for(int i=0;i<NCORES;i++)
        som+=msg.finalizou[i];
    
    return som==NCORES;
}

void ImgParalela::eGetDados(void* dados, uint32_t tamanho) {
    while(!eFinalizou());
    
    e_read(&emem,0,0,(off_t)(DESLOCAMENTO_DADOS+IMAGEM_ALTURA*IMAGEM_LARGURA*2),dados,tamanho);
}


void ImgParalela::setProf(cv::Mat img) {
    imgProf=img;
}

void ImgParalela::adicionaCorBola(unsigned char R, unsigned char G, unsigned char B) {
    if(qtdAmostras>=2047)return ;

    uint16_t posicao;
    posicao= ((uint16_t)(R & 0xF8)<<0x8)|((uint16_t)(G & 0xFC)<<0x3)|(uint16_t)(B>>3);
    if(!coresBola[posicao/0x8]&&(0x1<<(posicao%0x8)))qtdAmostras++;
    coresBola[posicao/0x8]|=(0x1<<(posicao%0x8));
       

   // std::cout<<"("<<(uint16_t)R<<","<<(uint16_t)G<<","<<(uint16_t)B<<")\t"<<posicao<<"\n";
 //   std::cout<<((uint16_t)(R & 0xF8)<<0x8)<<" "<<((uint16_t)(G & 0xFC)<<0x3)<<" "<<(B>>3)<<"\n\n";
}

 int ImgParalela::getProf(int x, int y) {
        return imgProf.cols>x&&imgProf.rows>y?(unsigned int)imgProf.at<uint16_t>(y,x):-1;
       
}
 /*
inline bool ImgSequencial::verifCorBola(const Cor& cor) {
    if(!cor.estaEntre(verticeMenor,verticeMaior))
        return false;  //<< otimização aqui !!!!!!!!!!!
    //caso não esteja no cubo retorna, se não pesquisa binariamente.
    return std::binary_search(coresBola.begin(),coresBola.end(),cor);
}
*/
void ImgParalela::binariza() {
    eProcessa(BINARIZAR,imgRGB.data,imgRGB.rows*imgRGB.step);

#if MOSTRAR_VIDEO
        eGetDados(imgBin.data,imgBin.rows*imgBin.step);
#else
    while(!eFinalizou());
#endif
}

bool ImgParalela::getCentro(int* x, int* y) {  ///<<-- implementar
    uint32_t res[NCORES][3];
    
    eProcessa(GETCENTRO,NULL,0);
    while(eFinalizou());
    eGetDados(&res,sizeof(res));
    
    int somaX=0,somaY=0,nElementos=0;
    for(int i=0;i<NCORES;i++){
        somaX      +=  res[i][0];
        somaY      +=  res[i][1];
        nElementos +=  res[i][2];
    }
    if(!nElementos)return false;
    *x=somaX/nElementos;
    *y=somaY/nElementos;
 
    return true;
}

/*
bool ImgSequencial::getCentro(int* x, int* y) {  ///<<-- media ponderada
    
   double pesoX=0,pesoY=0,somaX=0,somaY=0;
   int elementosBola;
//#pragma omp parallel for  reduction(+:somaX,somaY,elementosBola) 
    int nLinhas=imgBin.rows;  //determina as linhas 
    int nColunas=imgBin.cols;//determina as colunas sendo que chanel é 1
//#pragma omp parallel for  reduction(+:somaX,somaY,elementosBola)
    for(int j=0;j<nLinhas;j++){//y
        uint8_t *linha=imgBin.ptr<uint8_t>(j);
        for(int i=0;i<nColunas;i++){//x
            if(linha[i]){
                double peso;
                
                peso=(double)1/ *x<i?(i-*x):(*x-i);
                pesoX+=peso;
                somaX+=i*peso;
                
                peso=(double)1/ *y<j?(j-*y):(*y-j);
                pesoY+=peso;
                somaY+=j*peso;
            }
	}
    }
    if(elementosBola==0)return false;   //se a imagem é inteira preta  já diz que não encontrou
    *x=(int)(somaX/pesoX);
    *y=(int)(somaY/pesoY);
    
    return true;
}
*/
unsigned int ImgParalela::getQtdAmostras() {
    eProcessa(COPIARVET,&coresBola,0x2000);
    while(!eFinalizou());
    return qtdAmostras;//coresBola.size();
}

cv::Mat ImgParalela::getRGB() {
    return imgRGB.clone();
}

void ImgParalela::filtrarRuido() {
    cv::Mat imgFiltrada(IMAGEM_ALTURA,IMAGEM_LARGURA,CV_8UC1);  
    cv::Mat erodeElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(3,3));  
    cv::Mat dilateElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(5,5));
    cv::erode(imgBin,imgFiltrada,erodeElement);
    cv::dilate(imgFiltrada,imgBin,dilateElement);
    
}


ImgParalela::~ImgParalela() {
    e_close(&edev);
    e_free(&emem);
    e_finalize();
   
    
    imgBin.release();
    imgProf.release();
    imgRGB.release();
   // coresBola.clear();
}




