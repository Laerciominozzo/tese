/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cond.h
 * Author: lminozzo
 *
 * Created on 3 de Fevereiro de 2017, 18:13
 */

#ifndef COND_H
#define COND_H
#include"Mutex.h"


class Cond {
private:
    pthread_cond_t cond;
   
public:
    Cond();
    void espera(Mutex* mutex);
    void sinaliza();
    virtual ~Cond();
private:

};

#endif /* COND_H */

