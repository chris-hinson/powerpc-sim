# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CC = g++
CFLAGS = -Wall -g --std=c++17

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o
	$(CC) $(CFLAGS) -o ./out/emu ./out/main.o

main.o: main.cpp
	$(CC) $(CFLAGS) -o ./out/main.o -c main.cpp

clean:
	rm main.o
