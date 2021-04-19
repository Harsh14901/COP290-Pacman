SHELL = /bin/bash

CC = g++
C_FLAGS = -g -Wall

BIN  = output
SRCS = src/*.cpp
PROG = output/main
SRC = src
INCLUDE := include
LIB := lib
LFLAGS = -lSDL2 -lGL -lGLEW -lSDL2_image -lSDL2_ttf -lSDL2_mixer






EXECUTABLE = main

all: $(BIN)/$(EXECUTABLE)

clean: 
	$(RM) $(BIN)/$(EXECUTABLE)

run: all 
	./$(BIN)/$(EXECUTABLE) $(args)
$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp

	$(CC) $(C_FLAGS) -I$(INCLUDE) -I ./src/ -L$(LIB) $^ $(LFLAGS) -o $@ 
