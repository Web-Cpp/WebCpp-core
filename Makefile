CC = g++
FLAGS = -ggdb -std=c++17
LIBS = -lWs2_32
SRC = ./src/*.cpp ./src/http/*cpp
BUILD = -o ./bin/webcpp.exe

all:
	$(CC) $(SRC) $(FLAGS) $(LIBS) $(BUILD)
	./bin/webcpp.exe