# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: clean compile link cleanObjAndrun

compile:
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp

link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/simulation *.o

clean: 
	rm -f bin/*

cleanObjAndrun:
	rm -f *.o
	clear
	./bin/simulation

