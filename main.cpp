/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:33
 */
#include"Comum.h"
#include"Aplicacao.h"
#include <signal.h>
#include <unistd.h>



bool *continua;
bool encerrar=false;
bool mostrarDados=false;
bool mostrarImagens=false;
bool calibrar=false;



void sinais(int sinal){
    switch(sinal){
        case 2:
            encerrar=true;
            std::cout<<"\n\nParando a aplicação!\n";
            break;
        case SIGALRM:
        case 10:
            mostrarDados=true;
            std::cout<<"\n\tESTADO ATUAL DO PROGRAMA\n";
            #ifdef MOSTRAR_DADOS_PERIODICAMENTE
            alarm(ALARME_TEMPO);
            #endif
            break;
        case 30:
            mostrarImagens=true;
            std::cout<<"\nMostrando imagens\n";
            break;
        case 16:
            calibrar=true;
            std::cout<<"\nCalibrando cores\n";
            break;
    }
    *continua=false;
}

int main(int argc, char** argv) {
    std::cout<<"Processo (PID): "<<getpid()<<std::endl;

    //-----------------------------------------------------------------------
    //              PROGRAMA DE VERDADE
    //-----------------------------------------------------------------------
    Aplicacao app;
    continua=&app.continua;
    
    signal(SIGINT,sinais);
    signal(10,sinais);
    signal(30,sinais);
    signal(SIGALRM,sinais);
    signal(16,sinais);
    
    app.getCoresBola();
#ifdef MOSTRAR_DADOS_PERIODICAMENTE
    alarm(ALARME_TEMPO); 
#endif 
#ifdef MASCARA
    std::cout<<"Usando a Mascara: 0x"<<std::hex<<MASCARA<<std::dec<<std::endl;
#endif
#ifdef HSV
    std::cout<<"Usando o espaço de cores HSV.\n";
#else
    std::cout<<"Usando o espaço de cores RGB.\n";
#endif
    do{
        std::cout<<"Processo (PID): "<<getpid()<<std::endl;
        if(calibrar){app.getCoresBola();calibrar=false;}                //sinal 16 ou iniciação
        if(mostrarDados){std::cout<<app.verDados();mostrarDados=false;} // sinal 10
        if(mostrarImagens){                                             //sinal 30
            cv::namedWindow("RGB",CV_WINDOW_AUTOSIZE);cv::namedWindow("Prof",CV_WINDOW_AUTOSIZE);
            app.mostrarImagens((char*)"RGB",(char*)"Prof");
            cv::waitKey();
            mostrarImagens=false;
        }
        
        app.continua=true;
        std::cout<<"Dados zerados\n";
        app.zerarDados();  //importante estar logo antes de app.corre() para não interferir nas medidas
        app.corre();
    }while(!encerrar);
    std::cout<<app.verDados();
    cv::destroyAllWindows();
    return 0;
}

