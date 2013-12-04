#
# makefile for Kevin Bacon Project
#

home    = /home/courses/cop4531p/fall13
cpp     = $(home)/cpp
tcpp    = $(home)/tcpp
proj    = .

incpath = -I$(proj) -I$(cpp) -I$(tcpp)

CC      = g++47 -std=c++11 -Wall -Wextra

#all:  kevinbacon.x

symgraph.o : $(proj)/symgraph.h
	$(CC) -o symgraph.o $(proj)/symgraph.h

moviematch.o : $(proj)/moviematch.h
	$(CC) -o moviematch.o $(proj)/moviematch.h
	
kevinbacon.o : $(proj)/kevinbacon.cpp
	$(CC) -o kevinbacon.o $(proj)/kevinbacon.cpp

kevinbacon.x: $(proj)/symgraph.o $(proj)/moviematch.o $(proj)/kevinbacon.o
	$(CC) -o kevinbacon.x $(proj)/symgraph.o $(proj)/moviematch.o $(proj)/kevinbacon.o
