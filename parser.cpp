#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "headers/lex.h"
#include "headers/parser.h"

using namespace std;

void Parser::AST_clear(AST_node* AST_tree_node){
	if (AST_tree_node == NULL){
		return;
	}
	AST_clear(AST_tree_node->left);
	AST_clear(AST_tree_node->right);
	delete AST_tree_node;
	return;
}

void Parser::show_AST(AST_node* AST_tree, int space){
	if(AST_tree == NULL){
		
		return;
	}
	if((AST_tree->token.get_token_type() == number_type) || (AST_tree->token.get_token_type() == id_type)){
		for (int i = 0; i < space; ++i)
			cout << "\t";
		if(AST_tree->token.get_token_type() == number_type)
			cout << "<number>" << AST_tree->token.get_token_name() << "</number>" << endl;
		if(AST_tree->token.get_token_type() == id_type)
			cout << "<id>" << AST_tree->token.get_token_name() << "</id>" << endl;
			
		return;
	}
	for (int i = 0; i < space; ++i)
		cout << "\t";
	cout << "<expr op=\"" << AST_tree->token.get_token_name() << "\">" << endl;
	show_AST(AST_tree->left, space + 1);
	show_AST(AST_tree->right, space + 1);
	for (int i = 0; i < space; ++i)
		cout << "\t";
	cout << "</expr>" << endl;
	
	return;
}

Parser::Parser(){
	precedences[plus_type] = 10;
	precedences[minus_type] = 10;
	precedences[mul_type] = 20;
	precedences[div_type] = 20;
	precedences[pow_type] = 30;
	
	asoc[plus_type] = l_asoc;
	asoc[minus_type] = l_asoc;
	asoc[mul_type] = l_asoc;
	asoc[div_type] = l_asoc;
	asoc[pow_type] = r_asoc;
	
	AST_tree = NULL;
}

Parser::~Parser(){
	precedences.clear();
	asoc.clear();
	out_stack.clear();
	op_stack.clear();
	AST_clear(AST_tree);
	while(AST_stack.size() != 0){
		AST_clear(AST_stack[AST_stack.size() - 1]);
	}
	
	//cout << "\n\nParser destruct\n\n";
}

Token Parser::build_AST_tree(vector<Token>& tokens){
	int i = 0;
	int paren_counter = 0;
	Token token(0, -1, "Nan");
	Token next_token(0, -1, "Nan");
	
	/*for(i = 0; tokens[i].get_token_type() != endl_type; ++i){
		tokens[i].show_token();
	}*/
	for (i = 0; tokens[i].get_token_type() != endl_type; ++i){
		if(tokens[i].get_token_type() == open_paren_type){
			++paren_counter;
		}
		if(tokens[i].get_token_type() == close_paren_type){
			--paren_counter;
		}
	}
	
	if(paren_counter < 0){
		return log_err_expected("(");
		//return 1;
	} else if (paren_counter > 0) {
		return log_err_expected(")");
		//return 1;
	}
	
	
	next_token = tokens[0];
	//for(i = 0; (token = tokens[i]).get_token_type() != endl_type; ++i){
	for(i = 0; (token = next_token).get_token_type() != endl_type; ++i){
		next_token = tokens[i + 1];
		if ((token.get_token_type() == number_type) || (token.get_token_type() == id_type)){
			if((witch_group(next_token.get_token_type()) != operation_group) && (next_token.get_token_type() != close_paren_type) && (next_token.get_token_type() != endl_type)){
				return log_err_expected_met("operation", next_token);
				//return 1;
			}
			out_stack.push_back(token);
		} else if (precedences[token.get_token_type()] > 0){
			if((witch_group(next_token.get_token_type()) != operand_group) && (next_token.get_token_type() != open_paren_type)){
				return log_err_expected_met("operand", next_token);
				//return 1;
			}
			while ((op_stack.size() > 0) && ((precedences[op_stack[op_stack.size() - 1].get_token_type()] > precedences[token.get_token_type()]) || ((precedences[op_stack[op_stack.size() - 1].get_token_type()] == precedences[token.get_token_type()]) && (asoc[token.get_token_type()] == l_asoc))) && (op_stack[op_stack.size() - 1].get_token_type() != open_paren_type)){
				out_stack.push_back(op_stack[op_stack.size() - 1]);
				op_stack.pop_back();
			}
			op_stack.push_back(token);
		} else if (token.get_token_type() == open_paren_type){
			if((witch_group(next_token.get_token_type()) != operand_group) && (next_token.get_token_type() != open_paren_type) ){
				return log_err_expected_met("operand", next_token);
				//return 1;
			}
			op_stack.push_back(token);
		} else if (token.get_token_type() == close_paren_type){
			if((witch_group(next_token.get_token_type()) != operation_group) && (next_token.get_token_type() != close_paren_type) && (next_token.get_token_type() != endl_type)){
				return log_err_expected_met("operation", next_token);
				//return 1;
			}
			while (op_stack[op_stack.size() - 1].get_token_type() != open_paren_type){
				out_stack.push_back(op_stack[op_stack.size() - 1]);
				op_stack.pop_back();
				if (op_stack.size() == 0){
					//cout << "Error: ( expected\n\n";
					return log_err_expected("(");
					//return 1;
				}
			}
			op_stack.pop_back();
		}
	}
	
	while(op_stack.size() > 0){
	out_stack.push_back(op_stack[op_stack.size() - 1]);
	op_stack.pop_back();
	}

	//cout << "\n\n";
	
	out_stack.push_back(Token(-1, endl_type, "END"));
	/*for(i = 0; out_stack[i].get_token_type() != endl_type; ++i){
		cout << out_stack[i].get_token_name();
	}
	cout << "\n\n";*/
	/*out_stack[i].show_token();
	cout << "\n\n";*/
	
	for(i = 0; (token = out_stack[i]).get_token_type() != endl_type; ++i){
		if((token.get_token_type() == number_type) || (token.get_token_type() == id_type)){
			AST_tree = new AST_node;
			/*AST_node_ptr->token_type = token.get_token_type();
			AST_node_ptr->lexeme = token.get_token_name();*/
			AST_tree->token = token;
			AST_tree->left = NULL;
			AST_tree->right = NULL;
			AST_stack.push_back(AST_tree);
			continue;
		} else {
			AST_tree = new AST_node;
			/*AST_node_ptr->token_type = token.get_token_type();
			AST_node_ptr->lexeme = token.get_token_name();*/
			AST_tree->token = token;
			AST_tree->left = AST_stack[AST_stack.size() - 2];
			AST_tree->right = AST_stack[AST_stack.size() - 1];
			AST_stack.pop_back();
			AST_stack.pop_back();
			AST_stack.push_back(AST_tree);
			continue;
		}
		
		cout << "\n\nError:";
	}
	
	if(AST_stack.size() == 0){
		//cout << "\n\n Collapse" << endl;
		//return 1;
		return Token(-1 , exp_error, "Error: empty string");
	}
	
	AST_tree = AST_stack[AST_stack.size() - 1];
	AST_stack.pop_back();
	//show_AST(AST_node_ptr, 0);
	//return 0;
	
	return Token(-1 , no_error, "");
}

void Parser::show_AST_tree(){
	show_AST(AST_tree, 0);
}

int Parser::witch_group(int token_type){ //pow_type, mul_type, div_type, plus_type, minus_type
	if ((token_type == pow_type) || (token_type == mul_type) || (token_type == div_type) || (token_type == plus_type) || (token_type == minus_type)){
		return operation_group;
	}
	if ((token_type == number_type) || (token_type == id_type)){
		return operand_group;
	}
	
	return unknown_group;
}

Token Parser::log_err_expected(string expected){
	//cout << "Error: '" << expected << "' expected";
	string error_name = "";
	
	error_name += "Error: '";
	error_name += expected;
	error_name += "' expected";
	return Token(-1, exp_error, error_name);
}

Token Parser::log_err_expected_met(string expected, Token token_met){
	//cout << "Error(" << token_met.get_token_pointer_posistion() << "): \'" << expected << "\' expected, but \'"<< token_met.get_token_name() << "\' met";
	string error_name = "";
	
	error_name += "Error(";
	error_name += token_met.get_token_pointer_posistion();
	error_name += "): \'";
	error_name += expected;
	error_name += "\' expected, but \'";
	error_name += token_met.get_token_name();
	error_name += "\' met";
	return Token(-1, exp_error, error_name);
}













