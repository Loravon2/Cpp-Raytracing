CXX       := /usr/bin/g++
CXX_FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g

SRC     		:= source
INCLUDE 		:= include
DEF					:= defines
EIGEN   		:= Eigen
OPENCV_INC  := /usr/include/opencv4
OPENCV_CORE	:= opencv_core 
OPENCV_IMG  := opencv_imgcodecs

all: main.exe

main.exe: main.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@ -I$(INCLUDE) -I$(DEF) -I$(EIGEN) -I$(OPENCV_INC) -l$(OPENCV_CORE) -l$(OPENCV_IMG)

run: all
	./main.exe

clean:
	rm -f *.exe