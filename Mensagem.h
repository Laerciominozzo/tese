/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mensagem.h
 * Author: lminozzo
 *
 * Created on 13 de Abril de 2017, 16:21
 */

#ifndef MENSAGEM_H
#define MENSAGEM_H

#include "Comum.h"



#define NCORES 16
#define DESLOCAMENTO_DADOS  24 //Este deslocamento é necessário porque o sizeof retorna valores diferentes em cada dispositivo
                               //24d e 17h. Por esta razão foi definido 25 de deslocamento para os dados.
    enum{
        BINARIZAR,
        GETCENTRO,
        COPIARVET,
    };
    typedef struct{
        uint8_t finalizou[NCORES];      //<<--lembrar de zerar estes bytes
        uint8_t operacao;
    }Mensagem;
    




#endif /* MENSAGEM_H */

