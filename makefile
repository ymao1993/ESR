#compiler
CC = g++

#compiling flag
INCLUDE_PATH = /usr/local/include
CCFLAG = -c -Wall -std=c++11 -O3  -I $(INCLUDE_PATH)

#linking flag
LIB_OPENCV =  -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect
CLFlAG =  $(LIB_OPENCV)


all: ESRTest ESRTrain

ESRTest: ESRTest.o ESRRegressor.o
	$(CC) $(CLFlAG) -o ESRTest ESRTest.o ESRRegressor.o

ESRTrain: ESRTrain.o ESRRegressor.o $(CLFlAG )
	$(CC) $(CLFlAG) -o ESRTrain ESRTrain.o ESRRegressor.o

ESRTest.o: ESRTest.cpp
	$(CC) $(CCFLAG) ESRTest.cpp

ESRTrain.o: ESRTrain.cpp
	$(CC) $(CCFLAG) ESRTrain.cpp

ESRRegressor.o: ESRRegressor.cpp
	$(CC) $(CCFLAG) ESRRegressor.cpp

clean: 
	rm -f *.o
	rm -f ESRTest
	rm -f ESRTrain

