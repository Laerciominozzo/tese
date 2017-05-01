/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Aplicacao.h
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:41
 */

#ifndef APLICACAO_H
#define APLICACAO_H


#include "Img.h"
#include "JanelaCalibracao.h"
#include "MeuKinect.h"
#include "Comum.h"
#include "Tempos.h"
#include "Temporizador.h"
#include<string>

/// class Aplicacao - 
class Aplicacao {
  
public:
  bool continua;
private:
  MeuKinect* meuKinect;
  Freenect::Freenect freenect;    //objetos
  Img* img;
  Temporizador temporizador;
  Tempos tempos;
                    
  unsigned long int qtdTotal;
  unsigned long int qtdRGBPerdidas;
  unsigned long int qtdProfPerdidas;
  unsigned long int qtdRGBNaoRetornadas; //dados relevantes ao processamento
  unsigned long int qtdProfNaoRetornadas;
  
  unsigned long int qtdBolaNaoEncontrada;
  bool bolaEncontrada;

  int posBolaX;
  int posBolaY;
  int profBola;
  

  // Operations
public:
  Aplicacao ();
  void corre ();
  void getCoresBola ();
  std::string verDados ();
  void zerarDados();
  void mostrarImagens(char* nomeJanelaRGB,char* nomeJanelaBin);
  ~Aplicacao ();
};


#endif /* APLICACAO_H */

