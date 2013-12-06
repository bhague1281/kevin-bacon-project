#
# makefile for Kevin Bacon Project
#

home    = /home/courses/cop4531p/fall13
cpp     = $(home)/cpp
tcpp    = $(home)/tcpp
proj    = .

incpath = -I$(proj) -I$(cpp) -I$(tcpp)

CC      = g++47 -std=c++11 -Wall -Wextra $(incpath)

all:  kevinbacon.x

kevinbacon.x: $(proj)/kevinbacon.o $(proj)/bitvect.o $(proj)/primes.o $(proj)/hashfunctions.o $(proj)/xstring.o
	$(CC) -o kevinbacon.x kevinbacon.o bitvect.o primes.o hashfunctions.o xstring.o

kevinbacon.o: $(proj)/symgraph.h $(proj)/moviematch.h $(proj)/kevinbacon.cpp
	$(CC) -c $(proj)/kevinbacon.cpp

bitvect.o : $(cpp)/bitvect.h $(cpp)/bitvect.cpp
	$(CC) -c $(cpp)/bitvect.cpp

primes.o : $(cpp)/primes.h $(cpp)/primes.cpp
	$(CC) -c $(cpp)/primes.cpp

hashfunctions.o : $(cpp)/hashfunctions.h $(cpp)/hashfunctions.cpp
	$(CC) -c $(cpp)/hashfunctions.cpp

xstring.o : $(cpp)/xstring.h $(cpp)/xstring.cpp
	$(CC) -c $(cpp)/xstring.cpp
