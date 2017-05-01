/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeuKinect.h
 * Author: lminozzo
 *
 * Created on 25 de Janeiro de 2017, 21:40
 */

#ifndef MEUKINECT_H
#define MEUKINECT_H


#include <mutex>

#include <condition_variable>
#include "Temporizador.h"
#include "Tempos.h"
#include <libfreenect.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>


/// class MeuKinect - herda de Freenect::FreenectDevice

class MeuKinect : public Freenect::FreenectDevice {
private:
    cv::Mat imgRGB;
    cv::Mat imgProf;
    std::mutex mutexRGB;
    std::mutex mutexProf;
    std::condition_variable cond;
    Temporizador temporizador;
    Tempos tempos;

    int quadrosPerdidosRGB;
    int quadrosPerdidosProf;
    // Operations
public:

    MeuKinect(freenect_context* ctx, int index);
    void VideoCallback(void* _rgb, uint32_t timestamp);
    void DepthCallback(void* _depth, uint32_t timestamp);
    int getRGB(cv::Mat& img);
    int getRGBCompactado(cv::Mat& img);
    int getProf(cv::Mat& img);
    void inicia();
    void finaliza();
    void zerarDados();
    std::string verTempos();
    ~MeuKinect();
};

#endif /* MEUKINECT_H */

