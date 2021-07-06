#pragma once

#include "../tables/SymbolTable.h"
#include <vector>

// class to hold information about conditionals
class Conditional {
public:
	// constructor takes in name of control register, qubit index, and type of conditional (if,else,elsif)
	Conditional(string qvar_control, string control_index = "None", string cvalue = "IF") {
		control = qvar_control;
		index = control_index;
		value = cvalue;
	}

	// default constructor
	Conditional() = default;

	// add nested conditional
	void push_nested_cndtl(Conditional ctrl) {
		nested_condtls.push_back(ctrl);
	}

	// get nested conditional
	vector<Conditional> get_nested_cndtl() {
		return nested_condtls;
	}

	// get control
	string get_control() {
		return this->control;
	}


private:
	vector<Conditional> nested_condtls;
	string control;
	string index;
	string value;

};


