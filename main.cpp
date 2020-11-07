#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "headers/lex.h"
#include "headers/parser.h"

using namespace std;

int main()
{
	Lexer lexer("   1 + 2 * 3 ^ 4 ^ 0 / ( a + b ) ");
	Token token(0, -1, "Nan");
	vector<Token> tokens;
	Token build_unsuccess = Token(); 
	
	while((token = lexer.get_next_token()).get_token_type() != endl_type){
		if(token.get_token_type() == unknown_type){
			cout << "Error: unknown token type " << endl;
			token.show_token();
			tokens.clear();
			return -1;
		}
		tokens.push_back(token);
	}
	tokens.push_back(Token(-1, endl_type, "END"));
	tokens.push_back(Token(-1, endl_type, "END"));
	
	Parser parser;
	
	build_unsuccess = parser.build_AST_tree(tokens);
	
	if (build_unsuccess.get_token_type() == no_error){
		parser.show_AST_tree();
	} else {
		//cout << "\nBAD\n";
		cout << build_unsuccess.get_token_name() << endl;
	}
	
	tokens.clear();
	
	return 0;
}
