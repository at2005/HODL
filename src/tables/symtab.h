#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../compiler/garbage.h"
#include <string>
#include <unordered_map>
#include "../circuit/circuit.h"


class SymbolTable {
public:
	
	SymbolTable() : ax_counter(0), cmp_counter(0), ancilla_counter(0), hash_table({}), classical_hash_table({}), parent_scope(nullptr) {}

	//adds classical variable to classical stack of current scope
	void push_classical_var(ClassicalVariable* c_var) {
		this->classical_hash_table.insert({ c_var->get_name(), c_var });
	}

	//adds quantum variable to quantum stack of current scope
	void push_quantum_var(QuantumVariable* qvar) {
		QuantumVariable* garbage_reg = Garbage::get_garbage()->get_garbage_register(qvar->get_num_qubits());
		if (garbage_reg != NULL) {
			qvar->set_qreg(garbage_reg->get_qreg());
		}

		this->hash_table.insert({qvar->get_name(), qvar});
		Circuit::get_circuit()->add_qreg_list(qvar);
		
	}

	void add_to_garbage(QuantumVariable* qvar) {
		Garbage::get_garbage()->add_garbage(qvar);
	}
	

	//allows data in nested scopes to be referenced to in current scope
	void add_existing_table(SymbolTable* parent) {
		this->parent_scope = parent;
	}

	SymbolTable* get_parent_table() {
		return this->parent_scope;
	}

	//returns data from classical variable stack
	ClassicalVariable* search_ctable(string cvar_name) {
		ClassicalVariable* solution;
		auto iter = classical_hash_table.find(cvar_name);
		if (iter != classical_hash_table.end()) {
			solution = iter->second;
			return solution;
		}

		return NULL;
	}

	bool cvar_exists(string name) {
		if (classical_hash_table.find(name) != classical_hash_table.end()) return true;
		return false;
	
	}

	void delete_qvar(string name) {
		this->hash_table.erase(name);
	}

	//returns data from quantum variable stack
	QuantumVariable* search_qtable(string qvar_name) {
		QuantumVariable* solution;
		auto iter = hash_table.find(qvar_name);
		if (iter != hash_table.end()) {
			solution = iter->second;
			return solution;
		}

		if (parent_scope) {
			return parent_scope->search_qtable(qvar_name);
		}
		return NULL;
	}

	bool is_searchable(string name) {
		if (this->hash_table.find(name) != this->hash_table.end()) return true;
		return false;
	}

	void ADD_AX_REGISTER() {
		string var_name = "ax" + to_string(ax_counter);
		QuantumVariable* ax = new QuantumVariable(var_name);
		this->push_quantum_var(ax);
		this->ax_counter++;
		
	}

	string GET_AX_REGISTER() {
		if (ax_counter != 0) return "ax" + to_string(ax_counter - 1);
	}

	void ADD_CMP_REGISTER() {
		string var_name = "cmp" + to_string(cmp_counter);
		QuantumVariable* cmp = new QuantumVariable(var_name);
		cmp->set_num_qubits(1);
		this->push_quantum_var(cmp);
		this->cmp_counter++;
	}

	string GET_CMP_REGISTER() {
		if (cmp_counter != 0) return "cmp" + to_string(cmp_counter - 1);
	}

	void ADD_ANCILLA_REGISTER(int num_qubits) {
		
		string var_name = "ancilla" + to_string(ancilla_counter);
		QuantumVariable* ancilla = new QuantumVariable(var_name);
		ancilla->set_num_qubits(num_qubits);
		this->push_quantum_var(ancilla);
		this->ancilla_counter++;
	}

	string GET_ANCILLA_REGISTER() {
		if (ancilla_counter != 0) return "ancilla" + to_string(ancilla_counter - 1);

	}

	std::unordered_map<string, QuantumVariable*>& get_qtable() {
		return this->hash_table;
	}




private:
	std::unordered_map<string, QuantumVariable*> hash_table;
	std::unordered_map<string, ClassicalVariable*> classical_hash_table;
	unsigned int ax_counter;
	unsigned int cmp_counter;
	unsigned int ancilla_counter;
	SymbolTable* parent_scope;
	
};

#endif

