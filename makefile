CC=g++
CFLAGS= -std=c++0x -I.

assign3: functions.cpp main.cpp
	$(CC) -o assign3 functions.cpp main.cpp $(CFLAGS).

