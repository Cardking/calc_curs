#include <iostream>
#include <string>
#include "headers/lex.h"

using namespace std;

int Token::get_token_type(){
	return token_type;
}

int Token::get_token_pointer_posistion(){
	return token_pointer_posistion;
}

string Token::get_token_name(){
	return lexeme;
}


Token::Token(int pointer_position ,int type, string name) 
	: token_pointer_posistion(pointer_position), token_type(type), lexeme(name)
{}

Token::Token(){
} 

void Token::show_token(){
	cout << "Token-<" << this->token_pointer_posistion << " | " << this->token_type << ">\t" << this->lexeme << endl;
}




Lexer::Lexer(string in_line) : line(in_line){
	pointer_position = 0;
}

int Lexer::what_char(char this_char){
	if((this_char == ' ') || (this_char == '\t') || (this_char == '\n')){
		return space_char;
	}
	if(this_char >= '0' && this_char <= '9'){
		/*if (this_char != '0'){
			return nzdigit_char;
		}*/
		return digit_char;
	}
	if((this_char >= 'a' && this_char <= 'z') || (this_char >= 'A' && this_char <= 'Z') || (this_char == '_')){
		return __letter_char;
	}
	/*if((this_char >= '\'' && this_char <= '/') || (this_char >= ':' && this_char <= '>')){
		return punct_char;
	}*/
	
	return unknown_char;
}
	
Token Lexer::get_next_token(){
	string word = "";
	int i = 0;
	
	
	for ( ; what_char(line[pointer_position]) == space_char; ++pointer_position );
	if( line[pointer_position] == '\0' )
		return Token(pointer_position, endl_type, "END");
	switch(what_char(line[pointer_position])){
		case __letter_char:
			word += line[pointer_position];
			//++pointer_position;
			for(i = 1; (what_char(line[pointer_position + i]) == __letter_char) || (what_char(line[pointer_position + i]) == digit_char); ++i)
				word += line[pointer_position + i];
			pointer_position += i;
			
			return Token(pointer_position - i, id_type, word);
			break;
			
		case digit_char:
			if (line[pointer_position] != '0'){
				word += line[pointer_position];
				//cout << what_char(line[pointer_position + i]) << endl;
				for(i = 1; what_char(line[pointer_position + i]) == digit_char; ++i)
					word += line[pointer_position + i];
				if(line[pointer_position + i] == '.'){
					word += '.';
					++i;
					if(what_char(line[pointer_position + i]) == digit_char){
						for( ; what_char(line[pointer_position + i]) == digit_char; ++i)
							word += line[pointer_position + i];
						pointer_position += i;
						
						return Token(pointer_position - i, number_type, word);
					} else {
						pointer_position += i;
						
						return Token(pointer_position - i, unknown_type, word);
					}
				}
				if(what_char(line[pointer_position + i]) == __letter_char){
					for( ; (what_char(line[pointer_position + i]) == digit_char) || (what_char(line[pointer_position + i]) == __letter_char); ++i)
					word += line[pointer_position + i];
					pointer_position += i;
					return Token(pointer_position - i, unknown_type, word);
				}
				
				pointer_position += i;
				return Token(pointer_position - i, number_type, word);
			} else {
				word += line[pointer_position];
				if ( (what_char(line[pointer_position + 1]) == digit_char) || (what_char(line[pointer_position + 1]) == __letter_char)){
					//++pointer_position;
					for(i = 1; (what_char(line[pointer_position + i]) == __letter_char) || (what_char(line[pointer_position + i]) == digit_char); ++i)
						word += line[pointer_position + i];
					pointer_position += i;
					
					return Token(pointer_position - i, unknown_type, word);
				}
				++pointer_position;
				return Token(pointer_position - 1, number_type, word);
			}
			break;
		case unknown_char:
			switch((int)line[pointer_position]){
				case '/':
					++pointer_position;
					
					return Token(pointer_position - 1, div_type, "/");
					break;
				case '^':
					++pointer_position;
					
					return Token(pointer_position - 1, pow_type, "^");
					break;
				case '*':
					++pointer_position;
					
					return Token(pointer_position - 1, mul_type, "*");
					break;
				case '-':
					++pointer_position;
					
					return Token(pointer_position - 1, minus_type, "-");
					break;
				case '+':
					++pointer_position;
					
					return Token(pointer_position - 1, plus_type, "+");
					break;
				case '(':
					++pointer_position;
					
					return Token(pointer_position - 1, open_paren_type, "(");
					break;
				case ')':
					++pointer_position;
					
					return Token(pointer_position - 1, close_paren_type, ")");
					break;
				default:
					++pointer_position;
					
					return Token(pointer_position - 1, unknown_type, word += line[pointer_position]);
					break;
			}
		/*case digit_char:
			word += line[pointer_position];
			if ( (what_char(line[pointer_position + 1]) == digit_char) || (what_char(line[pointer_position + 1]) == __letter_char)){
				//++pointer_position;
				for(i = 1; (what_char(line[pointer_position + i]) == __letter_char) || (what_char(line[pointer_position + i]) == digit_char); ++i)
					word += line[pointer_position + i];
				pointer_position += i;
				
				return Token(pointer_position - i, unknown_type, word);
			}
			++pointer_position;
			return Token(pointer_position - 1, number_type, word);
			break;*/
	}
	++pointer_position;
					
	return Token(pointer_position - 1, unknown_type, word += line[pointer_position]);
}

void Lexer::change_line(string new_line){
	line = new_line;
	pointer_position = 0;
}
