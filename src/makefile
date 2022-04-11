# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g --std=c++17

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o
	$(CC) $(CFLAGS) -o emu main.o 

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

#Point.o: Point.h

#Square.o: Square.h Point.h
