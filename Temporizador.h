/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Temporizador.h
 * Author: lminozzo
 *
 * Created on 6 de Fevereiro de 2017, 15:52
 */

#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H
#include<time.h>
#include<sys/time.h>
#include<string>
#include<map>
/// class Temporizador - 
class Temporizador {
  // Attributes
private:

  std::map<std::string,struct timeval> ultimasLeituras;
 
  // Operations
public:
  Temporizador ();
  void marcarInicio (std::string chave);
  double marcarFim(std::string chave);
  ~Temporizador ();
};



#endif /* TEMPORIZADOR_H */

