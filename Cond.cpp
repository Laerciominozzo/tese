/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cond.cpp
 * Author: lminozzo
 * 
 * Created on 3 de Fevereiro de 2017, 18:13
 */

#include <pthread.h>
#include "Cond.h"


Cond::Cond() {
    pthread_cond_init(&cond,NULL);
}

void Cond::espera(Mutex *mutex) {
    pthread_cond_wait(&cond,mutex->get());
}

void Cond::sinaliza() {
    pthread_cond_signal(&cond);
}

Cond::~Cond() {
    pthread_cond_destroy(&cond);
}




