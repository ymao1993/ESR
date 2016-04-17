#compiler
CC = g++

#compiling flag
INCLUDE_PATH = /usr/local/include
CCFLAG = -c -Wall -std=c++11 -O3 -I $(INCLUDE_PATH)

#linking flag
LIB_OPENCV =  -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect
LDFlAG =  $(LIB_OPENCV)

OBJFILES = ESRRegressor.o ESRUtils.o ESRCascadedFerns.o ESRFern.o ESRBbox.o ESRFaceDetector.o

all: ESRTest ESRTrain

ESRTest:  ESRTest.o  $(OBJFILES)
	@echo linking...
	@$(CC) $(LDFlAG) -o ESRTest  ESRTest.o $(OBJFILES)

ESRTrain: ESRTrain.o $(OBJFILES)
	@echo linking...
	@$(CC) $(LDFlAG) -o ESRTrain ESRTrain.o $(OBJFILES)

-include $(OBJFILES:.o=.d)

%.o: %.cpp
	@echo compiling $*.cpp...
	@$(CC) $(CCFLAG) $*.cpp -o $*.o
	@$(CC) $(CCFLAG) -MM $*.cpp > $*.d

clean: 
	rm -f *.o
	rm -f ESRTest
	rm -f ESRTrain
	rm -f *.d
