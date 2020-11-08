all: main.o lex.o parser.o
	g++ -std=c++11 main.o lex.o parser.o -o main
main.o:
	g++ -c main.cpp -o main.o
lex.o:
	g++ -c lex.cpp -o lex.o
parser.o:
	g++ -c parser.cpp -o parser.o

clean:
	rm --force *.o main
