/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cor.h
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:44
 */

#ifndef COR_H
#define COR_H
#include<stdint.h>
/// class Cor - 
class Cor {
  // Attributes
private:
    union dados{
        uint32_t corID; //o union serve como otimização para comparações. 
        uint8_t RGB[4];   //Usar sempre corID
    }cor;     //foi padronizado este tipo de dado para garantir o tamanho dos campos nas diferentes arquiteturas
              //como são feitas operações de bits tem que garantir que o inteiro será maior que o vetor de char
  // Operations
public:
  Cor (unsigned char R, unsigned char G, unsigned char B);
  inline const bool operator > (const Cor&  p){return  getCorID()>p.getCorID();};
  inline const bool operator < (const Cor&  p){return  getCorID()<p.getCorID();};
  inline const bool operator == ( const Cor& p){return  getCorID()==p.getCorID();};
  inline const bool estaEntre ( Cor p1, Cor p2){return (p1.getB()<=getB()&&p1.getG()<=getG()&&p1.getR()<=getR()
                                                 && p2.getB()>=getB()&&p2.getG()>=getG()&&p2.getR()>=getR());};
  inline unsigned char getR (){return cor.RGB[0];};
  inline unsigned char getG (){return cor.RGB[1];};
  inline unsigned char getB (){return cor.RGB[2];};
  inline unsigned  int getCorID()const{return cor.corID;};
  void setR(unsigned char R);
  void setG(unsigned char G);
  void setB(unsigned char B);
  ~Cor ();
};


#endif /* COR_H */

