#ifndef DATA_H
#define DATA_H

#include <vector>
#include  "../parser/node.h"
#include <map>
// Data class is parent class for both QuantumVariable and ClassicalVariable

class Data {
public:
	
	// two constructors
	// Constructor 1 takes variable name
	Data(string data_name) :  num_qubits_required(new int(0)) {set_name(data_name); };
	// constructor 2 takes no name
	Data() : name(""), num_qubits_required(new int(0)) { *num_qubits_required = 0; };


	//increments number of qubits by 1
	void add_qubit() {
		(*this->num_qubits_required)++;
	}

	// increase register size by arbitrary amount
	void add_qubits(int qubits) {
		*this->num_qubits_required += qubits;
	}

	// set name
	void set_name(string new_name) {
		this->name = new_name;
	}

	//gets data name (identifier)
	string get_name() {
		return this->name;
	}

	//returns number of qubits required
	int get_num_qubits() {
		if (this) {
			return *this->num_qubits_required;
		}

		return 0;
	}

	// get pointer to number of qubits so that resources can be shared upon reassignment
	int* get_num_pointer() {
		if (this) {
			return this->num_qubits_required;
		}
	}

	// set pointer
	void set_num_pointer(int* ptr) {
		this->num_qubits_required = ptr;
	}

	//sets the number of qubits
	void set_num_qubits(int num) {
		*this->num_qubits_required = num;
	}

protected:

	string name;
	
	int* num_qubits_required;
};



//class for classical data
class ClassicalVariable : public Data {
public:
	// takes classical variable name
	ClassicalVariable(string c_variable):Data(c_variable) {}

	//sets classical value
	void set_value(double new_value) {
		this->value = new_value;
	}


	//gets classical value
	double get_value() {
		return this->value;
	}


private:
	// value that this container holds
	double value = 0;

};






//class for quantum data
class QuantumVariable : public Data {
public:
	QuantumVariable(string q_variable) : Data(q_variable), qreg(q_variable), references_in({}), references_out({}), flag(0), phys_start(0) {}
	
	QuantumVariable() : Data(), references_in({}), references_out({}), flag(0), phys_start(0) {}
	
	void set_name(string new_name) {
		this->name = new_name;
		
	}

	void set_qreg(string new_qreg) {
		this->qreg = new_qreg;
	}



	string get_qreg() {
		return this->qreg;
	}
	

	std::map<QuantumVariable*, Node*> get_dependency_map() {
		return this->dependency_map;
	}

	Node* get_dependency_expr(QuantumVariable* qvar) {
		return dependency_map.find(qvar)->second;
	}

	void set_dependency(QuantumVariable* qvar, Node* expr) {
		this->dependency_map.insert({ qvar, expr });
	}

	void add_reference(QuantumVariable* qvar) {
		this->references_in.push_back(qvar);
		qvar->references_out.push_back(this);
	}

	vector<QuantumVariable*> get_carry_ref() {
		return this->references_in;
	}

	vector<QuantumVariable*> get_given_ref() {
		return this->references_out;
	}

	bool flag;

	unsigned int get_phys_start() {
		return this->phys_start;
	

	}

	void set_phys_start(unsigned int phys) {
		this->phys_start = phys;

	}

private:
	std::map<QuantumVariable*, Node*> dependency_map;
	vector<QuantumVariable*> references_in;
	vector<QuantumVariable*> references_out;
	string qreg;
	unsigned int phys_start;



};



#endif


