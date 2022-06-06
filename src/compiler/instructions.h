#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>
#include "../tables/data.h"
#include "conditionals.h"
using namespace std;


class OPERATION {

public:
	OPERATION(string reg0_in, string reg1_in, string type_in, string result_in) {
		reg0 = reg0_in;
		reg1 = reg1_in;
		result = result_in;
		type = type_in;
	}



	OPERATION() : reg0(""), reg1(""), result(""), type("") {};

	string get_reg0() {
		return reg0;
	}

	string get_reg1() {
		return reg1;
	}

	string get_result() {
		return result;
	}

	string get_type() {
		return type;
	}

	

private:
	string result;
	string reg0;
	string reg1;
	string type;
	


};


class PREDEF_FUNC {
public:
	PREDEF_FUNC() : parameters({}), func_name("") {}
	PREDEF_FUNC(string name, string param) : parameters({ param }), func_name(name) {}
	PREDEF_FUNC(string name, vector<string> params) : parameters(params), func_name(name) {}

	vector<string> get_parameters() {
		return this->parameters;
	}

	string get_func_name() {
		return this->func_name;
	}

private:
	vector<string> parameters;
	string func_name;

};


class INSTRUCTION {
public:
	INSTRUCTION(string reg0, string reg1, string type, string result) {
		OP =  OPERATION(reg0, reg1, type, result);
		PF = PREDEF_FUNC();
		value = "OPERATION";
		is_inverse = false;
	}

	INSTRUCTION(string reg0, string reg1, string type, string result, vector<Conditional> controls_in) {
		OP = OPERATION(reg0, reg1, type, result);
		PF = PREDEF_FUNC();
		value = "OPERATION";
		controls = controls_in;
		is_inverse = false;
	
		
	}

	INSTRUCTION(string func_name, string input) {
		value = "PREDEF_FUNC";
		OP = OPERATION();
		PF = PREDEF_FUNC(func_name, input);
		is_inverse = false;
		
	}

	INSTRUCTION(string func_name, vector<string> parameters) {
		value = "PREDEF_FUNC";
		OP = OPERATION();
		PF = PREDEF_FUNC(func_name, parameters);
		is_inverse = false;

	}

	INSTRUCTION(string func_name, vector<string> parameters, vector<Conditional> controls_in) {
		value = "PREDEF_FUNC";
		OP = OPERATION();
		PF = PREDEF_FUNC(func_name, parameters);
		controls = controls_in;
		is_inverse = false;
		
	}


	INSTRUCTION(SymbolTable* current_scope, string type) {
		OP = OPERATION();
		PF = PREDEF_FUNC();
		controls = {};
		is_inverse = false;
		scope = current_scope;
		value = type;
	}

	OPERATION GET_OP() {
		
		if (value == "OPERATION") {
			return OP;
		}

		else {
			cerr << "NOT AN OP";
		}

	}

	PREDEF_FUNC GET_PF() {
		if (value == "PREDEF_FUNC") {
			return PF;
		}

		else {
			cerr << "NOT A PREDEF_FUNC";
		}
	}

	SymbolTable* get_table() {
		return this->scope;
	}

	string get_value() {
		return this->value;
	}

	vector<Conditional> get_controls() {
		return this->controls;
	}

	INSTRUCTION inverse() {
		INSTRUCTION instruction = *this;
		instruction.is_inverse = true;
		return instruction;
	}
	
	bool is_inverted() {
		return is_inverse;
	}

	void print() {
		if (value == "OPERATION") {
			cout << OP.get_reg0() << endl;
			cout << OP.get_type() << endl;
			cout << OP.get_reg1() << endl;
			if (this->is_inverted()) cout << "Inverse\n";
			cout << OP.get_result() << endl << endl << endl;
			cout << "-------------------\n";


		}

		else if (value == "BEGIN" || value == "END") {
			cout << value << " : " << scope << endl;
			cout << "-------------------\n";
		}

		else {
			cout << PF.get_func_name() << endl;
			if (this->is_inverted()) cout << "Inverse\n";
			for (int i = 0; i < PF.get_parameters().size(); i++) {
				cout << PF.get_parameters()[i] << endl;
			}
			cout << "-------------------\n";
			cout << endl << endl;
		}
	}

private:
	bool is_inverse;
	string value;
	OPERATION OP;
	PREDEF_FUNC PF;
	vector<Conditional> controls;
	SymbolTable* scope;
};

#endif
