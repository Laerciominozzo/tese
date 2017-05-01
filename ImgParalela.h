/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImgSequencial.h
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:45
 */

#ifndef IMGSEQUENCIAL_H
#define IMGSEQUENCIAL_H

#include "Comum.h"
#include "Img.h"
#include "Cor.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <e-hal.h>
 
#define BUF_OFFSET 0x01000000
// não quis recriar uma mascara para caso não tenha sido definida para não atrapalhar outras partes do codigo
/// class ImgSequencial - 
class ImgParalela : public Img {
 

private:
  cv::Mat imgRGB;
  cv::Mat imgProf;
  cv::Mat imgBin;
  uint8_t coresBola[0x2000];   //vetor unidimensional que vai receber o mapa de bits
          
  unsigned int qtdAmostras;
  
  e_platform_t eplat;
  e_epiphany_t edev;
  e_mem_t emem;

  

 // Cor verticeMaior;
  //Cor verticeMenor;
  // Operations
public:
  ImgParalela ();
  
  void setRGB (cv::Mat img);
  void setProf (cv::Mat img);
  void binariza ();
  cv::Mat getBin ();
  void adicionaCorBola (unsigned char R, unsigned char G, unsigned char B);
  bool getCentro (int* x, int* y);
  int getProf (int x, int y);
  unsigned int getQtdAmostras();    //nova
  cv::Mat getRGB();     //nova
  void filtrarRuido();
  
  ~ImgParalela ();
private:
  /// procura a cor da bola na estrutura
  /// @param ponto		(???) 
  //inline bool verifCorBola ( Cor cor);
  void eProcessa(uint8_t op, void* dados, uint32_t tamanho);
  bool eFinalizou();
  void eGetDados(void* dados, uint32_t tamanho);
};


#endif /* IMGSEQUENCIAL_H */

