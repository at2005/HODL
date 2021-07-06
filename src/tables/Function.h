#ifndef FUNCTION_H
#define FUNCTION_H

#include "data.h"
#include "../parser/SyntaxTree.h"
//class for functions
class Function  {
public:
	//constructor assigns name to "name" variable
	Function(SyntaxTree* st) : name(st->getRoot()->getTValue()), function_definition(st), classical_bool(false),oracle(false) {
		
	}

	Function() : classical_bool(false), function_definition(nullptr), name(""),oracle(false) {
	}

	//returns function definition
	SyntaxTree* get_syntax_tree() {
		return this->function_definition;
	}

	vector<SyntaxTree> getFuncBody() {
		return this->function_definition->get_child_trees();
	}


	//sets true if function is purely classical
	void set_classical() {
		this->classical_bool = true;
	}

	//returns true if function is truly classical
	bool is_classical() {
		return this->classical_bool;
	}

	string get_name() {
		return this->name;
	}

	void set_oracle() {
		this->oracle = true;
	}

	bool is_oracle() {
		return this->oracle;
	}

private:
	SyntaxTree* function_definition;
	bool classical_bool;
	string name;
	bool oracle;

};

#endif
