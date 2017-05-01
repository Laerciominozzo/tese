/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mutex.cpp
 * Author: lminozzo
 * 
 * Created on 25 de Janeiro de 2017, 21:36
 */

#include "Mutex.h"

Mutex::Mutex(){
    pthread_mutex_init(&mutex,NULL);
}

void Mutex::bloc(){
    pthread_mutex_lock(&mutex);
}

void Mutex::desbloc() {
    pthread_mutex_unlock(&mutex);
}

pthread_mutex_t* Mutex::get() {
    return &mutex;
}

bool Mutex::tBloc() {
    return !pthread_mutex_trylock(&mutex);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex);
}
