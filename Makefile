 all: aplicacao e_aplicacao

OPENCV_LIBS=`pkg-config --libs opencv`
OPENCV_FLAGS=`pkg-config --cflags opencv`
FREENECT_INCLUDES=-I/usr/local/include/libfreenect -I/usr/include/libusb-1.0/
DEFINES= -D RESOLUCAO_MEDIA -D MOSTRAR_VIDEO
CFLAGS= -std=c++11 -Os   

E_HAL_INCLUDES= -I $(EPIPHANY_HOME)/tools/host/include/
E_LIBS= -L $(EPIPHANY_HOME)/tools/host/lib/
ELF=$(EPIPHANY_HOME)/bsps/current/internal.ldf

e_aplicacao: eMain.c
	e-g++ -Os -T $(ELF) eMain.c -o e_aplicacao -le-lib  $(DEFINES)
	
aplicacao : MeuKinect.o main.o Mutex.o JanelaCalibracao.o Img.o ImgParalela.o  Aplicacao.o Cond.o Temporizador.o Tempos.o
	g++ MeuKinect.o main.o Mutex.o JanelaCalibracao.o Img.o ImgParalela.o Aplicacao.o Cond.o Temporizador.o Tempos.o -o aplicacao -lfreenect $(OPENCV_LIBS) $(CFLAGS) $(E_LIBS) -le-hal -le-loader -lpthread

main.o : main.cpp Aplicacao.h Comum.h teste.cpp
	g++ -c main.cpp -o main.o $(OPENCV_FLAGS)  $(DEFINES) $(FREENECT_INCLUDES) $(CFLAGS)

MeuKinect.o : MeuKinect.cpp MeuKinect.h Mutex.h Comum.h Cond.h
	g++ -c MeuKinect.cpp  $(OPENCV_FLAGS) $(FREENECT_INCLUDES) $(DEFINES) $(CFLAGS)

Mutex.o : Mutex.cpp Mutex.h Comum.h
	g++ -c Mutex.cpp -lpthread $(DEFINES) $(CFLAGS)

JanelaCalibracao.o : JanelaCalibracao.cpp JanelaCalibracao.h Comum.h
	g++ -c JanelaCalibracao.cpp $(OPENCV_FLAGS) $(DEFINES) $(CFLAGS)
	
Img.o : Img.cpp Img.h Comum.h
	g++ -c Img.cpp $(DEFINES) $(OPENCV_FLAGS) $(CFLAGS)
	
ImgParalela.o : ImgParalela.cpp ImgParalela.h Img.h Comum.h
	g++ -c ImgParalela.cpp $(DEFINES) $(OPENCV_FLAGS) $(CFLAGS) $(E_HAL_INCLUDES) -le-loader

Cor.o : Cor.cpp Cor.h Comum.h
	g++ -c Cor.cpp $(DEFINES) $(CFLAGS)
	
Aplicacao.o : Aplicacao.cpp Aplicacao.h MeuKinect.h ImgParalela.h JanelaCalibracao.h Tempos.h Comum.h
	g++ -c Aplicacao.cpp $(DEFINES) $(OPENCV_FLAGS) $(FREENECT_INCLUDES) $(CFLAGS) $(E_HAL_INCLUDES)
	
Cond.o : Cond.cpp Cond.h Mutex.h Comum.h
	g++ -c Cond.cpp $(DEFINES) $(CFLAGS)
	
Temporizador.o: Temporizador.cpp Temporizador.h Comum.h
	g++ -c Temporizador.cpp $(DEFINES) $(CFLAGS)
	
Tempos.o: Tempos.h Tempos.cpp Comum.h
	g++ -c Tempos.cpp $(DEFINES) $(CFLAGS)
	
clean: 
	rm -f *.o aplicacao e_aplicacao
