#pragma once
#include "../tables/symtab.h"
#include "../interpreter/interpreter.h"
#include "../circuit/circuit.h"
#include "../general/base_conversion.h"
#include <map>


 unsigned long long eval_resources(SyntaxTree* tree, SymbolTable* table, QuantumVariable* dependency = nullptr, Node* expr_node = nullptr, bool is_right_child = false, map<string, string> parameter_map = {});

 std::map<string, string> number_to_word() {
	std::map<std::string, std::string> numbers;
	numbers.insert({ "0", "zero" });
	numbers.insert({ "1", "one" });
	numbers.insert({ "2", "two" });
	numbers.insert({ "3", "three" });
	numbers.insert({ "4", "four" });
	numbers.insert({ "5","five" });
	numbers.insert({ "6", "six" });
	numbers.insert({ "7", "seven" });
	numbers.insert({ "8", "eight" });
	numbers.insert({ "9", "nine" });
	numbers.insert({ "10", "ten" });
	numbers.insert({ "11", "eleven" });
	numbers.insert({ "12", "twelve" });
	numbers.insert({ "13", "thirteen" });
	numbers.insert({ "14", "fourteen" });
	numbers.insert({ "15", "fifteen" });
	numbers.insert({ "16", "sixteen" });
	numbers.insert({ "17", "seventeen" });
	numbers.insert({ "18", "eighteen" });
	numbers.insert({ "19", "nineteen" });
	numbers.insert({ "20", "twenty" });
	numbers.insert({ "30", "thirty" });
	numbers.insert({ "40", "forty" });
	numbers.insert({ "50", "fifty" });
	numbers.insert({ "60", "sixty" });
	numbers.insert({ "70", "seventy" });
	numbers.insert({ "80", "eighty" });
	numbers.insert({ "90", "ninety" });
	numbers.insert({ "100", "hundred" });
	numbers.insert({ "1000", "thousand" });
	numbers.insert({ "1000000", "million" });
	numbers.insert({ "1000000000", "billion" });
	numbers.insert({ "1000000000000", "trillion" });
	numbers.insert({ "1000000000000000", "quadrillion" });
	numbers.insert({ "1000000000000000000", "quintillion" });
	numbers.insert({ "1000000000000000000000", "sextillion" });
	numbers.insert({ "1000000000000000000000000", "septillion" });
	numbers.insert({ "1000000000000000000000000000", "octillion" });
	numbers.insert({ "1000000000000000000000000000000", "nonillion" });
	numbers.insert({ "100000000000000000000000000000000", "decillion" });
	numbers.insert({ "100000000000000000000000000000000000", "undecillion" });
	numbers.insert({ "100000000000000000000000000000000000000", "duodecillion" });
	numbers.insert({ "100000000000000000000000000000000000000000", "tredecillion" });
	numbers.insert({ "100000000000000000000000000000000000000000000", "quattuordecillion" });
	numbers.insert({ "100000000000000000000000000000000000000000000000", "quindecillion" });
	numbers.insert({ "1000000000000000000000000000000000000000000000000", "sexdecillion" });
	numbers.insert({ "100000000000000000000000000000000000000000000000000", "septendecillion" });

	return numbers;
}




// the function get_num_to_words converts a number to its word representation
 string get_num_to_words(string number) {
	string output = "";
	std::map<string, string> number_map = number_to_word();
	for (int i = 0; i < number.size(); i++) {
		string temp = "";
		temp += number[i];
		output += number_map.find(temp)->second;
	}

	return output;
}


 QuantumVariable max_var(QuantumVariable a, QuantumVariable b) {
	return a.get_num_qubits() > b.get_num_qubits() ? a : b;
}


 string getIdentifier(string left, string right) {
	return isIdentifier(left) ? left : right;
}

 string getNumeric(string left, string right) {
	return isNumeric(left) ? left : right;
}



 int estimate_addition(QuantumVariable qvar1, QuantumVariable qvar2) {
	return max_var(qvar1, qvar2).get_num_qubits() + 1;
}


 int estimate_addition(QuantumVariable qvar, int number) {
	int num_qubits = floor(log2(number)) + 1;
	if (num_qubits > qvar.get_num_qubits()) {
		return num_qubits + 1;
	}

	return qvar.get_num_qubits() + 1;
}


 int estimate_addition(int num_qubits1, int num_qubits2) {
	return max(num_qubits1, num_qubits2) + 1;
}




 void estimate_addition_append(QuantumVariable& qvar1, QuantumVariable qvar2) {
	qvar1.set_num_qubits(max_var(qvar1, qvar2).get_num_qubits() + 1);

}

 void estimate_addition_append(QuantumVariable& qvar, int num) {
	unsigned int num_qubits = floor(log2(num)) + 1;
	qvar.set_num_qubits(max<int>(num_qubits, num) + 1);
}

 int estimate_addition_append(int num_qubits1, int num_qubits2) {

	return (max<int>(num_qubits1, num_qubits2) + 1);
}



 int estimate_subtraction(QuantumVariable& qvar1, QuantumVariable qvar2) {
	if (qvar2.get_num_qubits() > qvar1.get_num_qubits()) {
		qvar1.set_num_qubits(qvar2.get_num_qubits());
	}

	return max_var(qvar1, qvar2).get_num_qubits();
}

 int estimate_subtraction(QuantumVariable& qvar, int num) {
	return qvar.get_num_qubits();
}

 int estimate_subtraction(int num_qubits1, int num_qubits2) {
	return max<int>(num_qubits1, num_qubits2);
}


 int estimate_subtraction_append(QuantumVariable& qvar1, QuantumVariable qvar2) {
	if (qvar2.get_num_qubits() > qvar1.get_num_qubits()) {
		qvar1.set_num_qubits(qvar2.get_num_qubits());
	}

	return qvar1.get_num_qubits();

}

 int estimate_subtraction_append(QuantumVariable& qvar1, int num) {
	int num_qubits = floor(log2(num)) + 1;
	if (num_qubits > qvar1.get_num_qubits()) {
		qvar1.set_num_qubits(num_qubits);
	}

	return qvar1.get_num_qubits();
}

 int estimate_subtraction_append(int num_qubits1, int num_qubits2) {
	return max<int>(num_qubits1, num_qubits2);
}


 int estimate_multiplication(QuantumVariable qvar1, QuantumVariable qvar2) {
	return qvar1.get_num_qubits() * 2;//(qvar1.get_num_qubits() + qvar2.get_num_qubits());
}



 int estimate_multiplication(int num_qubits1, int num_qubits2) {
	return ((num_qubits1 + num_qubits2) + 1);
}

 int estimate_int_mult(QuantumVariable qvar, int multiplier) {
	int number_of_bits = floor(log2(multiplier) + 1);
	return ((qvar.get_num_qubits() + number_of_bits));
}


 void estimate_int_mult_append(QuantumVariable& qvar, int multiplier) {
	int number_of_bits = floor(log2(multiplier) + 1);
	qvar.set_num_qubits((qvar.get_num_qubits() + number_of_bits) - 1);
}

 int estimate_range(int upper_bound) {
	return floor(log2(upper_bound - 1)) + 1;
}


QuantumVariable* create_num_reg(Circuit& qc, unsigned int num, int padding, SymbolTable& table) {
	QuantumVariable* number_register = new QuantumVariable(get_num_to_words(to_string(num)));

	string bin = to_binary(num, padding);
	number_register->set_num_qubits(bin.size());

	qc.add_qregister(*number_register);
	table.push_quantum_var(number_register);

	for (int i = 0; i < bin.size(); i++) {
		if (bin[i] == '1') {
			qc.x(number_register->get_qreg(), i);
		}
	}

	return number_register;
}















//evaluates number of qubits for classical data (numbers)
unsigned long long eval_classical_resources(Node* node) {
	string number = node->getTValue();
	return floor(log2(stoll(node->getTValue()))) + 1;
}





//evaluates number of qubits for quantum data (identifiers)
unsigned int long long eval_id_resources(Node* node, string left, string right, SymbolTable* table) {
	QuantumVariable qvar1 = *table->search_qtable(left);
	QuantumVariable qvar2 = *table->search_qtable(right);

	//estimate qubits for addition
	if (node->getTValue() == "+") {
		return estimate_addition(qvar1, qvar2);
	}

	//estimate qubits for subtraction
	else if (node->getTValue() == "-") {
		return estimate_subtraction(qvar1, qvar2);
	}

	//estimate qubits for multiplication
	else if (node->getTValue() == "*") {
		return estimate_multiplication(qvar1, qvar2);
	}

	else if (node->getTValue() == "+=") {
		estimate_addition_append(qvar1, qvar2);
		qvar1.set_dependency(&qvar2, node);
		return 0;
	}

	else if (node->getTValue() == "-=") {
		estimate_subtraction_append(qvar1, qvar2);
		qvar1.set_dependency(&qvar2, node);
		return 0;
	}

	return 0;
}


unsigned long long eval_id_num_resources(Node* node, string left, string right, SymbolTable* table) {
	QuantumVariable qvar = *table->search_qtable(getIdentifier(left, right));
	int number = stoi(getNumeric(left, right));
	//estimate number of qubits for addition
	if (node->getTValue() == "+") {
		return estimate_addition(qvar, number);
	}

	//estimate number of qubits for subtraction
	else if (node->getTValue() == "-") {
		return estimate_subtraction(qvar, number);
	}

	//estimate number of qubits for multiplication
	else if (node->getTValue() == "*") {
		return estimate_int_mult(qvar, number);
	}

	else if (node->getTValue() == "+=") {
		estimate_addition_append(qvar, number);
		return 0;
	}

	else if (node->getTValue() == "-=") {
		estimate_subtraction_append(qvar, number);
		return 0;
	}

	return 0;
}




void eval_operator_child(Node* node, SymbolTable* table, int& total_resources, Node* expr_node, QuantumVariable* dependency, map<string, string> parameter_map = {}) {
	int value;
	//dependency = nullptr;

	SyntaxTree left_tree(node->getLeftChild());
	SyntaxTree right_tree(node->getRightChild());

	if (node->getTValue() == "+") {
		value = estimate_addition(eval_resources(&left_tree, table, dependency, expr_node, false, parameter_map), eval_resources(&right_tree, table, dependency, expr_node, true, parameter_map));
		total_resources += value;
	}

	else if (node->getTValue() == "-") {
		value = estimate_subtraction(eval_resources(&left_tree, table, dependency, expr_node, false, parameter_map), eval_resources(&right_tree, table, dependency, expr_node, true, parameter_map));
		total_resources += value;
	}

	else if (node->getTValue() == "*") {
		value = estimate_multiplication(eval_resources(&left_tree, table, dependency, expr_node, false, parameter_map), eval_resources(&right_tree, table, dependency, expr_node, true, parameter_map));
		total_resources += value;
	}



	else if (node->getTValue() == ":") {
		total_resources += estimate_range(eval_resources(&right_tree, table, dependency, expr_node, false, parameter_map));

	}

	table->search_qtable(node->get_result_register())->set_num_qubits(value);


}
