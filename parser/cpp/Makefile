all: a.out

a.out: main.cpp parser.o
	g++ main.cpp parser.o -o a.out

parser.o: parser.cpp parser.hpp
	g++ -c parser.cpp -o parser.o

clean:
	rm a.out parser.o