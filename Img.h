/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Img.h
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:44
 */

#ifndef IMG_H
#define IMG_H
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Comum.h"

/// class Img - 
 class Img {
  // Operations
public:
//  virtual uint16_t* global_RGB();
  virtual void setRGB (cv::Mat img)=0;
  virtual void setProf (cv::Mat img)=0;
  virtual void binariza ()=0;
  virtual cv::Mat getBin ()=0;
  virtual void adicionaCorBola (unsigned char R, unsigned char G, unsigned char B)=0;
  virtual bool getCentro (int* x, int* y)=0;
  virtual  int getProf (int x, int y)=0;
  virtual unsigned int getQtdAmostras()=0;
  virtual cv::Mat getRGB()=0;
  virtual void filtrarRuido()=0;
  
  static void adicionaCorBola (void* obj, unsigned char R, unsigned char G, unsigned char B);
};

#endif /* IMG_H */

