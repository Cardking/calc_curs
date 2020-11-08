all: main.o lex.o parser.o
	g++ -std=c++11 main.o lex.o parser.o -o main
main.o:
	g++ -c main.cpp -o main.o
lex.o:
	g++ -c lex.cpp -o lex.o
parser.o:
	g++ -c parser.cpp -o parser.o
lexer_test: lexer_test.o lex.o
	g++ -std=c++11 tests/lexer_test.o lex.o -o test
lexer_test.o:
	g++ -std=c++11 -c tests/lexer_test.cpp -o tests/lexer_test.o
parser_test: parser_test.o parser.o lex.o
	g++ -std=c++11 tests/parser_test.o parser.o lex.o -o test
parser_test.o:
	g++ -std=c++11 -c tests/parser_test.cpp -o tests/parser_test.o
clean:
	rm --force *.o main test

	

