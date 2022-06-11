#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include "expr.h"
#include "parse_funcs.h"

class SymbolTable;

class SyntaxTree {
public:
	string get_expr_type() {
		return this->expression_type;
	}

	SyntaxTree(Node* node) {
		this->parent = nullptr;
		this->type = "NULL";
		this->root_of_tree = node;
		this->name = "ID";
		if (node->get_func_params().size()) {
			vector<vector<Pair>> expressions = returnSep(node->get_func_params());
			for (int i = 0; i < expressions.size(); i++) {
				SyntaxTree* st = new SyntaxTree(expressions[i]);
				this->function_parameters.push_back(*st);
			}
				
		}
	}

	

	SyntaxTree(vector<Pair> inputExpr) {
		//cout << "hello\n";
		this->type = "NULL";
		this->parent = nullptr;
		this->expression_type = rtnExprType(inputExpr);
		//cout << expression_type;
		this->root_of_tree = new Node();
		
		if (inputExpr.size() == 1 ) {
			*this->root_of_tree =  Node(inputExpr[0], inputExpr[0].getValue());
			this->name = "ID";
			
			
		}

		
		else if (inputExpr.size() == 2 && inputExpr[0].getToken() == "KEYWORD") {
			//cout << "this is a keyword";
		
			this->root_of_tree =  new Node(inputExpr[0], inputExpr[0].getValue());
			vector<Pair> id = { inputExpr[1] };
			SyntaxTree st(id);
			this->function_parameters.push_back(st);
			this->name = "KEYWORD_OP";
			
		}

		else if (expression_type == "VARIABLE_DECLARATION") {
		
			this->root_of_tree = createDeclTree(inputExpr);
			//cout << root_of_tree->getTValue();
			this->name = "ASSIGNMENT";

		}

		else if (expression_type == "OPERATION") {
			//cout << "hey";
			this->root_of_tree = parse_maths(inputExpr);
			this->name = "ASSIGMENT";
		}

		else if (expression_type == "KEYWORD_OPERATION") {
			//cout << "hey2";
			check = true;
			this->root_of_tree = createKeywordTree(inputExpr);
		}
		
		else if (expression_type == "FUNCTION_CALL") {
			//cout << "hey3";
			this->root_of_tree = createFCallTree(inputExpr);
			//cout << root_of_tree->getTValue();
			this->name = "FCALL";
		}

		else if (expression_type == "IF_STATEMENT" || expression_type == "ELSE_IF_STATEMENT") {
			//cout << "if";
			this->root_of_tree = createCndtlnTree(inputExpr);
			this->name = "CONDITIONAL";
		}

		else if (expression_type == "ELSE_STATEMENT") {
			this->root_of_tree = createElseTree(inputExpr);
			this->name = "ELSE";
		}

		else if (expression_type == "ORACLE_FUNCTION" || expression_type == "FUNCTION_DECLARATION") {
			//cout << "func";
			this->root_of_tree = createFuncDefTree(inputExpr);	
			if(expression_type == "FUNCTION_DECLARATION" ) this->name = "FUNC";
			if (expression_type == "ORACLE_FUNCTION") this->name = "ORACLE";
		}

		else if (expression_type == "FOR_LOOP") {

			this->root_of_tree = createForLoopTree(inputExpr);
			this->name = "FOR";
		}

		else if (expression_type == "WHILE_LOOP") {
			this->root_of_tree = createWhileLoopTree(inputExpr);
			this->name = "WHILE";
		}

		else {
			this->root_of_tree = NULL;
			this->name = "";
			//cout << "UNEXPECTED_SEMICOLON";
		}


	}





	Node* getRoot() {
		return this->root_of_tree;

	}

	
	SyntaxTree* get_parent() {
		return this->parent;

	}

	string getName() {
		return this->name;
	}


	vector<SyntaxTree>& get_child_trees() {
		return this->child_trees;
	}
	
	void set_child_trees(vector<SyntaxTree> children) {
		child_trees = {};
		for (auto& a : children) {
			child_trees.push_back(a);
		}
	}
	
	vector<SyntaxTree> get_function_parameters() {
		return this->function_parameters;
	}

	void setcf() {
		this->cond_flag = true;

	}

	bool getcf() {
		return this->cond_flag;

	}

	void set_table(SymbolTable* table) {
		this->current_scope = table;
	}


	SymbolTable* get_table() {
		return this->current_scope;
	}



private:
	bool check = false;
	bool cond_flag;
	//expression type
	string expression_type;
	string name;
	string type;
	//root of the Abstract-Syntax-Tree
	Node* root_of_tree;
	SymbolTable* current_scope;
	vector<SyntaxTree> child_trees = {};
	vector<SyntaxTree> function_parameters = {};
	SyntaxTree* parent;
	
	
	//parse body of code block
	void parse_body(vector<Pair> inputExpr) {
		vector<Pair> temp = parse_between("{", "}", inputExpr);

		auto vec = getEachExpression(temp);

		for (int i = 0; i < vec.size(); i++) {
			SyntaxTree* st = new SyntaxTree(vec[i]);
			st->parent = this;
			this->child_trees.push_back(*st);
		}
	}

	//Creates AST of variable declaration


	Node* createDeclTree(vector<Pair> inputExpr) {
		
		Node* root;
		if (inputExpr.size() == 1) {
			
			root = new Node(inputExpr[0], inputExpr[0].getValue());
		}


		else if (inputExpr.size() == 2) {
			root = new Node(inputExpr[1], inputExpr[0].getValue());
		}

		else { 
			//for(int i = 0; i < inputExpr.size(); i++) cout << inputExpr[i].getValue() << endl; 
			
			root =  new Node(Pair("=", "ASSIGNMENT"), "assignment");
			
			root->createLeftChild(inputExpr[1], inputExpr[0].getValue());
			vector<Pair> tempVec = {};
			int index = 0;
			for (int i = 0; i < inputExpr.size(); i++) {
				if (inputExpr[i].getValue() == "=") {
					index = i;
					break;
				}
			}

			for (int i = index + 1; i < inputExpr.size(); i++) {
				tempVec.push_back(inputExpr[i]);
			}
			
			//cout << tempVec.size();
			root->createRightChild(parse_maths(tempVec));
			
			

		}
		
		
		return root;

		
	}

	//Creates AST of keyword operation


	Node* createKeywordTree(vector<Pair> inputExpr) {
		
		Node* root =  new Node(inputExpr[0], inputExpr[0].getValue());

		for (int i = 1; i < inputExpr.size(); i++) {
			if (inputExpr[i].getToken() == "IDENTIFIER") {
				vector<Pair> individual_expr = {inputExpr[i]};
				SyntaxTree st(individual_expr);
				this->child_trees.push_back(st);
			}
		}
		return root;
	}

	//creates AST of function call

	Node* createFCallTree(vector<Pair> inputExpr) {
		//if(inputExpr[0].getValue() == "filter") for(int i = 0; i < inputExpr.size(); i++) cout << inputExpr[i].getValue() << endl; 
		Node* root = new Node(); 
		//if(inputExpr[0].getValue() == "filter")  cout << "\ndone\n";			
		*root = *(parse_maths(inputExpr));
		root->setTreeToken("FCALL");
		vector<vector<Pair>> tempVec = returnSep(root->get_func_params());
		for (int i = 0; i < tempVec.size(); i++) {
			SyntaxTree st = SyntaxTree(tempVec[i]);
			this->function_parameters.push_back(st);

		}

		//cout << this->function_parameters.size();
		//cout << root->getTValue() << ":" << this->function_parameters.size() << endl;
		return root;

	}

		
	Node* createCndtlnTree(vector<Pair> inputExpr) {
		Node* root =  new Node(inputExpr[0], inputExpr[0].getValue());
		root->createLeftChild(parse_maths(parse_between("(", ")", inputExpr)));
		parse_body(inputExpr);

		return root;
	}


	Node* createElseTree(vector<Pair> inputExpr) {
		Node* root = new Node(inputExpr[0], inputExpr[0].getValue());
		parse_body(inputExpr);
		return root;
	}

	
	Node* createFuncDefTree(vector<Pair> inputExpr) {
		string return_type = "NULL";
		if (inputExpr[0].getToken() == "TYPE") {
			return_type = inputExpr[1].getValue();
		}

		this->type = return_type;
		int index = 0;
		
		for (int i = 0; i < inputExpr.size(); i++) {
			if (inputExpr[i].getToken() == "IDENTIFIER" || inputExpr[i].getValue() == "main") {
				index = i;
				break;
			}
		}

		//cout << index;

		
		Node* root = new Node(inputExpr[index], inputExpr[0].getValue());
		
		vector<vector<Pair>> separated_values = returnSep(parse_between("(", ")", inputExpr));
	

		for (int i = 0; i < separated_values.size(); i++) {
			SyntaxTree st =  SyntaxTree(separated_values[i]);
			this->function_parameters.push_back(st);
		}

		parse_body(inputExpr);

		return root;

		
	}

	
		
	


	Node* createForLoopTree(vector<Pair> inputExpr) {
		Node* root = new Node(inputExpr[0], inputExpr[0].getValue());
		vector<vector<Pair>> tempVec = {};
		tempVec = returnSep(parse_between("(", ")", inputExpr));
		for (int i = 0; i < tempVec.size(); i++) {
			SyntaxTree st =  SyntaxTree(tempVec[i]);
			this->function_parameters.push_back(st);
		}

		parse_body(inputExpr);
		return root;


	}



	Node* createWhileLoopTree(vector<Pair> inputExpr) {
		Node* root = new Node(inputExpr[0], inputExpr[0].getValue());
		root->createLeftChild(parse_maths(parse_between("(", ")", inputExpr)));
		parse_body(inputExpr);
		return root;

	}

};

#endif
