/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Img.cpp
 * Author: lminozzo
 * 
 * Created on 25 de Janeiro de 2017, 21:44
 */

#include "Img.h"

void Img::adicionaCorBola(void* obj, unsigned char R, unsigned char G, unsigned char B) {
    Img* objeto=(Img*)obj;
    objeto->adicionaCorBola(R,G,B);
}
 /*Função auxiliar que serve apenas para poder chamar a função de callbac*/


