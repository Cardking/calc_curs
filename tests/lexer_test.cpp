#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/lex.h"


TEST_CASE("CheckLexer")
{
	Lexer lexer("nothing");

	lexer.change_line("123");
	REQUIRE(lexer.get_next_token().get_token_type() == number_type);
    
    lexer.change_line("0");
	REQUIRE(lexer.get_next_token().get_token_type() == number_type);
    
    lexer.change_line("12.3");
	REQUIRE(lexer.get_next_token().get_token_type() == number_type);
    
    lexer.change_line("name");
	REQUIRE(lexer.get_next_token().get_token_type() == id_type);
    
    lexer.change_line("_name");
	REQUIRE(lexer.get_next_token().get_token_type() == id_type);
    
    lexer.change_line("034");
	REQUIRE(lexer.get_next_token().get_token_type() == unknown_type);
    
    lexer.change_line("0mda");
	REQUIRE(lexer.get_next_token().get_token_type() == unknown_type);
    
    lexer.change_line("3mda");
	REQUIRE(lexer.get_next_token().get_token_type() == unknown_type);
    
    lexer.change_line("3432mda");
	REQUIRE(lexer.get_next_token().get_token_type() == unknown_type);
    
    lexer.change_line("+");
	REQUIRE(lexer.get_next_token().get_token_type() == plus_type);
    
    lexer.change_line("-");
	REQUIRE(lexer.get_next_token().get_token_type() == minus_type);
    
    lexer.change_line("*");
	REQUIRE(lexer.get_next_token().get_token_type() == mul_type);
    
    lexer.change_line("/");
	REQUIRE(lexer.get_next_token().get_token_type() == div_type);
    
    lexer.change_line("^");
	REQUIRE(lexer.get_next_token().get_token_type() == pow_type);
    
}



