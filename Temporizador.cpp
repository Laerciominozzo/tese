/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Temporizador.cpp
 * Author: lminozzo
 * 
 * Created on 6 de Fevereiro de 2017, 15:52
 */

#include "Temporizador.h"
#include "Comum.h"

Temporizador::Temporizador() {
    marcarInicio("Aplicação");
}

void Temporizador::marcarInicio(std::string chave) {
    gettimeofday(&ultimasLeituras[chave],0);
}

double Temporizador::marcarFim(std::string chave) {
    struct timeval fim;
    gettimeofday(&fim,0);         //pega o tempo agora
    double retorno=(fim.tv_sec+(double)fim.tv_usec/1000000)-(ultimasLeituras[chave].tv_sec+(double)ultimasLeituras[chave].tv_usec/1000000);
    marcarInicio(chave);
    return retorno;
}

Temporizador::~Temporizador() {
    ultimasLeituras.clear();
}
