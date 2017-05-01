/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tempos.h
 * Author: lminozzo
 *
 * Created on 10 de Fevereiro de 2017, 18:11
 */

#ifndef TEMPOS_H
#define TEMPOS_H
#include"Comum.h"
#include<map>
#include <vector>
#include<string>
#include<time.h>
#include <sys/times.h>
/// class Tempos - 
class Tempos {
  // Attributes
private:
  std::map<std::string,unsigned long int> qtdTempos;
  std::map<std::string,double> somTempos;
  // Operations
public:
  Tempos ();
  double setTempo (std::string chave, double tempo);
  std::string verDados ();
  std::string verDados (std::string chave);
  void zerarDados ();
  ~Tempos ();
};


#endif /* TEMPOS_H */

