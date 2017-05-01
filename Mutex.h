/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mutex.h
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:36
 */

#ifndef MUTEX_H
#define MUTEX_H

#include<pthread.h>
/// class Mutex - 

class Mutex {
  // Attributes
private:
    pthread_mutex_t mutex;
  // Operations
public:
    Mutex ();
    void bloc ();
    bool tBloc();
    void desbloc ();
    pthread_mutex_t* get();
    ~Mutex ();
};


#endif /* MUTEX_H */

