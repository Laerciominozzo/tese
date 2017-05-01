/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JanelaCalibracao.h
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:42
 */

#ifndef JANELACALIBRACAO_H
#define JANELACALIBRACAO_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
/// class JanelaCalibracao - 
class JanelaCalibracao {
  // Attributes
private:
  void* obj;
  void (*func)(void*,unsigned char,unsigned char,unsigned char);
  cv::Mat img;
  int selX,selY;
  bool selecionando;
  // Operations
public:
  JanelaCalibracao (void* obj, void (*func)(void*,unsigned char,unsigned char,unsigned char));
  bool mostrar(cv::Mat img);   //nova função
  ~JanelaCalibracao ();
private:
  static void mouseCallBack (int evento, int x, int y, int , void* This);
};


#endif /* JANELACALIBRACAO_H */

