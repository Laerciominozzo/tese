#include<iostream>
#include <unistd.h>

#include "Temporizador.cpp"
#include "Tempos.cpp"

int main(){
    Temporizador temporizador;
    Tempos tempos;
    
    temporizador.marcarInicio("Teste");
    sleep(1);
    tempos.setTempo("Teste",temporizador.marcarFim("Teste"));
    sleep(2);
    tempos.setTempo("Teste",temporizador.marcarFim("Teste"));
    tempos.setTempo("Aplicação",temporizador.marcarFim("Aplicação"));
    std::cout<<tempos.verDados()<<std::endl;    
    return 0;
}