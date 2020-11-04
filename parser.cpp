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

int Parser::build_AST_tree(vector<Token>& tokens){
	int i = 0;
	Token token(0, -1, "Nan");
	
	/*for(i = 0; tokens[i].get_token_type() != endl_type; ++i){
		tokens[i].show_token();
	}*/
	
	for(i = 0; (token = tokens[i]).get_token_type() != endl_type; ++i){
		if ((token.get_token_type() == number_type) || (token.get_token_type() == id_type)){
			out_stack.push_back(token);
		} else if (precedences[token.get_token_type()] > 0){
			while ((op_stack.size() > 0) && ((precedences[op_stack[op_stack.size() - 1].get_token_type()] > precedences[token.get_token_type()]) || ((precedences[op_stack[op_stack.size() - 1].get_token_type()] == precedences[token.get_token_type()]) && (asoc[token.get_token_type()] == l_asoc))) && (op_stack[op_stack.size() - 1].get_token_type() != open_paren_type)){
				out_stack.push_back(op_stack[op_stack.size() - 1]);
				op_stack.pop_back();
			}
			op_stack.push_back(token);
		} else if (token.get_token_type() == open_paren_type){
			op_stack.push_back(token);
		} else if (token.get_token_type() == close_paren_type){
			while (op_stack[op_stack.size() - 1].get_token_type() != open_paren_type){
				out_stack.push_back(op_stack[op_stack.size() - 1]);
				op_stack.pop_back();
				if (op_stack.size() == 0){
					cout << "Error: ( expected\n\n";
					return 1;
				}
			}
			op_stack.pop_back();
		}
	}
	
	while(op_stack.size() > 0){
	out_stack.push_back(op_stack[op_stack.size() - 1]);
	op_stack.pop_back();
	}

	cout << "\n\n";
	
	out_stack.push_back(Token(-1, endl_type, "END"));
	for(i = 0; out_stack[i].get_token_type() != endl_type; ++i){
		cout << out_stack[i].get_token_name();
	}
	cout << "\n\n";
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
		cout << "\n\n Collapse" << endl;
		return -1;
	}
	
	AST_tree = AST_stack[AST_stack.size() - 1];
	AST_stack.pop_back();
	//show_AST(AST_node_ptr, 0);
	return 0;
}

void Parser::show_AST_tree(){
	show_AST(AST_tree, 0);
}
