/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cor.cpp
 * Author: lminozzo
 * 
 * Created on 25 de Janeiro de 2017, 21:44
 */

#include "Cor.h"
#include "Comum.h"

Cor::Cor(unsigned char R, unsigned char G, unsigned char B) {
    cor.corID=0;        //garante que os bits restantes vao ser 0
    setB(B);
    setG(G);
    setR(R);
}
/*
bool Cor::estaEntre(Cor p1, Cor p2) {    //P1 tem que ser menor por questão de otimização
    return (p1.getB()<getB()&&p1.getG()<getG()&&p1.getR()<getR()
            && p2.getB()>getB()&&p2.getG()>getG()&&p2.getR()>getR());
}

unsigned char Cor::getB() {
    return cor.RGB[2];
}

unsigned char Cor::getG() {
    return cor.RGB[1];
}

unsigned char Cor::getR() {
    return cor.RGB[0];
}
*/
void Cor::setB(unsigned char B) {
#ifndef MASCARA
    cor.RGB[2]=B;
#else
        cor.RGB[2]=B & MASCARA;
#endif
   
}

void Cor::setG(unsigned char G) {
#ifndef MASCARA
    cor.RGB[1]=G;
#else
        cor.RGB[1]=G & MASCARA;
#endif
}

void Cor::setR(unsigned char R) {
#ifndef MASCARA
    cor.RGB[0]=R;
#else
        cor.RGB[0]=R & MASCARA;
#endif
}

/*
bool Cor::operator<(const Cor& p)const {
    return  getCorID()<p.getCorID();  
}
 
bool Cor::operator>(const Cor& p)const {
    return getCorID()>p.getCorID();
}

bool Cor::operator==(const Cor& p)const {
    return getCorID()==p.getCorID();
}

unsigned  int Cor::getCorID()const {
    return cor.corID;
}
*/
Cor::~Cor() {

}



