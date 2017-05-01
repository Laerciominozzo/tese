/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tempos.cpp
 * Author: lminozzo
 * 
 * Created on 10 de Fevereiro de 2017, 18:11
 */


#include<stdio.h>
#include "Tempos.h"
#include <unistd.h> 
Tempos::Tempos() {

}

double Tempos::setTempo(std::string chave, double tempo) {
    if(somTempos.find(chave)==somTempos.end()){        //caso seja o primeiro elemento do vetor já coloca a posição em 0
        qtdTempos[chave]=0;
        somTempos.insert(std::pair<std::string,double>(chave,0.0));
    }

    qtdTempos[chave]+=1;
    somTempos[chave]+=tempo;
    
    return tempo;
}

std::string Tempos::verDados() {
    std::string retorno="Metodo \t tempo total / quantidade de amostras = tmedia de tempo por amostra\n\n";
       
    for(std::map<std::string,double >::iterator i=somTempos.begin();i!=somTempos.end();i++){
        retorno+=verDados(i->first);
    }
        
    return retorno;
}

std::string Tempos::verDados(std::string chave) {           ///<<--verificar função, fiz enquanto estava meio dormindo
    std::string retorno=chave+"\t\t";
    if(somTempos.end()==somTempos.find(chave))return (retorno+"\n");
    
    char numero[50];
  
    sprintf(numero,"%lf",somTempos[chave]);
    retorno+=numero;
    retorno+=" / ";
    sprintf(numero,"%ld",qtdTempos[chave]);
    retorno+=numero;
    retorno+="\t=  ";
    sprintf(numero,"%lf",somTempos[chave]/qtdTempos[chave]);
    retorno+=numero;
    retorno+="\n";
        
    return retorno;
        
}

void Tempos::zerarDados() {

}

Tempos::~Tempos() {
    qtdTempos.clear();
    somTempos.clear();
}





