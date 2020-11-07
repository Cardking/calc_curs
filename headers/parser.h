#ifndef PARSER_H
#define PARSER_H

struct AST_node
{
    /*int token_type;
    string lexeme;*/
    Token token;
    
    AST_node* left;
    AST_node* right;
};

class Parser{
	private:
		
	map<int, int> precedences; //pow_type, mul_type, div_type, plus_type, minus_type
	map<int, int> asoc;
	vector<Token> out_stack;
	vector<Token> op_stack;
	vector<AST_node*> AST_stack;
	AST_node* AST_tree;
	
	void AST_clear(AST_node* AST_tree_node);	
	void show_AST(AST_node* AST_tree, int space);
	
	
	
	public:
	
	Parser();	
	~Parser();
	
	int build_AST_tree(vector<Token>& tokens);	
	void show_AST_tree();
	int witch_group(int token_type);
	void log_err_expected(string expected);
	void log_err_expected_met(string expected, Token token_met);
};

#endif
