LO = lex.o
PO = parser.o
TLO = tests/lexer_test.o
TPO = tests/parser_test.o
MO = main.o
CC = g++ -std=c++11 -o

all: main.o lex.o parser.o tests
	$(CC) parser $(MO) $(PO) $(LO)

main.o:
	$(CC) $(MO) -c main.cpp
lex.o:
	$(CC) $(LO) -c lex.cpp
parser.o:
	$(CC) $(PO) -c parser.cpp

lexer_test: lexer_test.o lex.o
	$(CC) lexer_test $(TLO) $(LO)
lexer_test.o:
	$(CC) $(TLO) -c tests/lexer_test.cpp
parser_test: parser_test.o parser.o lex.o
	$(CC) parser_test $(TPO) $(PO) $(LO)
parser_test.o:
	$(CC) $(TPO) -c tests/parser_test.cpp
	
tests: lexer_test parser_test
	./lexer_test
	./parser_test
	
.PHONY: run clean
clean:
	rm --force tests/*.o *.o parser test parser_test lexer_test
