CXX       := /usr/bin/g++
CXX_FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g

SRC     := source
INCLUDE := include
DEF			:= defines
EIGEN   := Eigen

all: clean main.exe

main.exe: main.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(DEF) -I$(EIGEN) $^ -o $@

test.exe: test.cpp $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(DEF) -I$(EIGEN) $^ -o $@

run: all
	./main.exe

clean:
	rm -f *.exe