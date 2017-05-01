/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   eMain.c
 * Author: lminozzo
 *
 * Created on 13 de Abril de 2017, 16:18
 */

#include "e_lib.h"
#include"Mensagem.h"
#include"Comum.h"

#define BUF_MENSAGEM   0x8f000000
#define BUF_CORES_BOLA 0x2000
#define BUF_IMAGEM_1   0x4000
#define BUF_IMAGEM_2   0x6000
#define BUF_DADOS (BUF_MENSAGEM+DESLOCAMENTO_DADOS)
#define IMAGEM_CORE_ALTURA IMAGEM_ALTURA/NCORES


typedef union{
uint16_t RGB[IMAGEM_CORE_ALTURA/2][IMAGEM_LARGURA];      //endereço onde será armazenadda a metade da imagem RGB
uint8_t  Bin[IMAGEM_CORE_ALTURA/2][IMAGEM_LARGURA];      //endereço onde será armazenada metade da imagem binaria
}img;

e_dma_desc_t dmaRGB;
e_dma_desc_t dmaBin;

uint8_t idcore;
img banco1 SECTION(".data_bank2");
img banco2 SECTION(".data_bank3");
uint8_t coresBola[0x2000] SECTION(".data_bank1");
 
inline uint16_t* getLinhaPartilhadoRGB(uint16_t linha){
    return (uint16_t*)(BUF_DADOS+(linha*NCORES+idcore)*IMAGEM_LARGURA*2); //<<--pensar na largura da linha
            //apartir do endereço partilhado+ larguraa da mensagem + (linha reqerida * a fatia que será para os outros nucleos + a fatia deste core)* o tamanho da linha 
}

inline uint8_t* getLinhaPartilhadoBin(uint16_t linha){
    return (uint8_t*)(BUF_DADOS+IMAGEM_ALTURA*IMAGEM_LARGURA*2+(linha*NCORES+idcore)*IMAGEM_LARGURA); //<<--pensar na largura da linha
            //apartir do endereço partilhado+ larguraa da mensagem + (linha reqerida * a fatia que será para os outros nucleos + a fatia deste core)* o tamanho da linha 
}
 uint16_t* getLinhaRGB(uint16_t linha){
     img* banco=linha%2?&banco1:&banco2;
     return &banco->RGB[linha/2][0];
 }
                                                //FAZER UMA REGRA PARA NÃO PEGAR LINHA QUE NÃO DEVE SER DESTE CORE
 uint8_t* getLinhaBin(uint16_t linha){
     img* banco=linha%2?&banco1:&banco2;
     return &banco->Bin[linha/2][0];
 }
 
uint16_t* getDMAlinhabloc(int linha){
    e_dma_copy((void*)getLinhaRGB(linha),(void*)getLinhaPartilhadoRGB(linha),IMAGEM_LARGURA*2);  //<<--fazer formula para pegar as linhas do core apenas
 }

void getDMAlinha(int linha){
    e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE | E_DMA_MASTER|E_DMA_DWORD),0x0,
            0x8 , 0x8,      //stride loop interno
            0x50 , 0x1,      //contador do loop interno e externo
            0x0 ,0x0,       //não utilizado (stride loop externo)
            (void*)getLinhaPartilhadoRGB(linha),(void*)getLinhaRGB(linha),&dmaRGB);
    e_dma_start(&dmaRGB,E_DMA_0);
}

void setDMAlinha(int linha){
    e_dma_set_desc(E_DMA_1,(E_DMA_ENABLE | E_DMA_MASTER|E_DMA_DWORD),0x0,
            0x8 , 0x8,      //stride loop interno
            0x28 , 0x1,      //contador do loop interno e externo
            0x0 ,0x0,       //não utilizado (stride loop externo)
            (void*)getLinhaBin(linha),(void*)getLinhaPartilhadoBin(linha),&dmaBin);
    e_dma_start(&dmaBin,E_DMA_1);
}
/*
 * 
 */
int main() {
    
    Mensagem msg;
   // banco1=(img*)BUF_IMAGEM_1;
  //  banco2=(img*)BUF_IMAGEM_2;
   // uint8_t *coresBola=(uint8_t*)BUF_CORES_BOLA;
    unsigned int coreLinha,coreColuna;
    e_coords_from_coreid(e_get_coreid(), &coreLinha, &coreColuna);
    idcore=coreLinha*4+coreColuna;
        
    while(1){
        e_dma_copy(&msg,(void*)BUF_MENSAGEM,sizeof(Mensagem));
        
        switch(msg.operacao){
            case COPIARVET:
                e_dma_copy(coresBola,(void*)(BUF_DADOS),0x2000);
                break;
            case BINARIZAR:
                getDMAlinhabloc(0); //espera pela 1º linha da imagem
                for(int i=0;i<IMAGEM_CORE_ALTURA;i++){
                    getDMAlinha(i+1);   // enquanto processa a linha atual já vai baxando a proxima(no fim pega uma amais.
                    uint16_t* linhaRGB=getLinhaRGB(i);
                    uint8_t*  linhaBin=getLinhaBin(i);
                    for(int j=0;j<IMAGEM_LARGURA;j++)
                        if(coresBola[linhaRGB[j]/0x8]&(0x1<<(linhaRGB[j]%0x8)))
                            linhaBin[j]=0xFF;
                        else
                            linhaBin[j]=0x00;
                    
             //   setDMAlinha(i);//como tem um barramento de DMA ocioso já manda a imagem binaria de volta
                }
             
#ifdef MOSTRAR_VIDEO           //não há mais a necessidade de ter este codigo por conta do DMA não bloquante
                for(int i=0;i<IMAGEM_CORE_ALTURA;i++)
                     e_dma_copy(getLinhaPartilhadoBin(i),getLinhaBin(i),IMAGEM_LARGURA);
#endif
  
                break;
            case GETCENTRO:
                uint32_t res[3]={0,0,0};
                uint32_t *nElementos=&res[2],*somaX=&res[0],*somaY=&res[1],nElementosLinha;
                for(int i=0;i<IMAGEM_CORE_ALTURA;i++){
                    uint8_t* linhaBin=getLinhaBin(i);
                    nElementosLinha=0;
                    for(int j=0;j<IMAGEM_LARGURA;j++)
                        if(linhaBin[j]){
                            nElementosLinha++;
                            *somaX+=j;
                        }
                    *nElementos+=nElementosLinha;
                    *somaY+=nElementosLinha*(i*16+idcore);
                    //soma y = a numero de elementos da linha * o numero da linha.
                }
                
                e_dma_copy((void*)(BUF_DADOS+IMAGEM_ALTURA*IMAGEM_LARGURA*2+idcore*sizeof(res)),&res,sizeof(res));
                break;
        }
        
        uint8_t val=1;
        
        e_dma_copy((void*)(BUF_MENSAGEM+idcore),&val,sizeof(val)); //avisa que acabou 
        __asm__ __volatile__ ("trap 4");
    }
    
}

