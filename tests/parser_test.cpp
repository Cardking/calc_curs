#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/lex.h"
#include "../headers/parser.h"
#include <iostream>
#include <vector>

using namespace std;

int build_token_vector(Lexer& lexer, vector<Token>& tokens){
	Token token;
	
	while((token = lexer.get_next_token()).get_token_type() != endl_type){
		if(token.get_token_type() == unknown_type){
			cout << "Error: unknown token type " << endl;
			token.show_token();
			tokens.clear();
			return 1;
		}
		tokens.push_back(token);
	}
	tokens.push_back(Token(-1, endl_type, "END"));
	tokens.push_back(Token(-1, endl_type, "END"));
	
	return 0;
}

TEST_CASE("CheckLexer")
{
	Lexer lexer("nothing");
	vector<Token> tokens;
    Token token;
	int i = 0;

	Parser parser;
    
    lexer.change_line("1 + 2 * 4 ^ 7 - (a + b)");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == no_error);
	
	lexer.change_line("(1 + 2 * 4 ^ 7 - (a + b)");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == exp_error);
	
	lexer.change_line("1 + 2 * 4 ^ 7 - (a + b))");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == exp_error);
	
	lexer.change_line("1 + 2 * * 4 ^ 7 - (a + b)");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == exp_error);
	
	lexer.change_line("1 + 2 54 * 4 ^ 7 - (a + b)");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == exp_error);
	
	lexer.change_line("1 + 2 * 4 ^ 7 - a + ");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == exp_error);
	
	lexer.change_line("1 + 2 * 4 ^ 7 - (+ a)");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == exp_error);
	
	lexer.change_line("1 + 2 * 4 ^ 7 - (a + b) c");
    tokens.clear();
    if (build_token_vector(lexer, tokens)){
		tokens.clear();
		return;
	}
	token = parser.build_AST_tree(tokens);
	REQUIRE(token.get_token_type() == exp_error);
	
	tokens.clear();
}



