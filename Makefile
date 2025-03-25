# Makefile for Comp390 TME 1 Programs

CC = g++
CFLAGS = -Wall -O2
LIBS = -lGL -lGLU -lglut -lm

all: program1 program2 program3

program1:
	$(CC) $(CFLAGS) -o Program1/program1 Program1/main.cpp $(LIBS)

program2:
	$(CC) $(CFLAGS) -o Program2/program2 Program2/main.cpp $(LIBS)

program3:
	$(CC) $(CFLAGS) -o Program3/program3 Program3/main.cpp $(LIBS)

clean:
	rm -f Program1/program1 Program2/program2 Program3/program3

.PHONY: all clean program1 program2 program3 