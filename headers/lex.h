#ifndef LEX_H
#define LEX_H

#include <string>

using namespace std;

enum {unknown_char ,__letter_char, punct_char, digit_char, nzdigit_char, space_char};
enum {unknown_type, endl_type, number_type, id_type, open_paren_type,
	close_paren_type, pow_type, mul_type, div_type, plus_type, minus_type,
	l_asoc, r_asoc};
enum {unknown_group ,operation_group, operand_group};
	
	
class Token{
	private:
	int token_pointer_posistion;
	int token_type;
	string lexeme;
	
	public:	
	int get_token_type();	
	int get_token_pointer_posistion();	
	string get_token_name();
	
	Token(int pointer_position ,int type, string name);	
	Token(); 
		
	void show_token();
};

class Lexer{
	private:
	unsigned int pointer_position;
	string line;
	
	public:
	Lexer(string in_line);
	
	int what_char(char this_char);
		
	Token get_next_token();
};

#endif
