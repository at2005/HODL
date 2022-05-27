#pragma once
#include <vector>
#include <string>
#include "../tables/data.h"
using namespace std;


class Garbage {
	 Garbage() {total_size = 0;}
	 static Garbage* garbage;
	 vector<QuantumVariable*> garbage_registers;
	 unsigned int total_size;


public:
	static Garbage* get_garbage() {
		if (!garbage) {
			garbage = new Garbage();
		}

		return garbage;
	}

	void add_garbage(QuantumVariable* qvar) {
		garbage_registers.push_back(qvar);
		total_size += qvar->get_num_qubits();
	}

	/*QuantumVariable* get_garbage_register(int register_size) {
		for (int reg = 0; reg < garbage_registers.size(); reg++) {
			if (garbage_registers[reg]->get_num_qubits() == register_size) {
				return garbage_registers[reg];
			}
		}

		return nullptr;
	}
*/


		
	
	QuantumVariable* get_recycled(unsigned int size, SymbolTable* table) {
		// check register reuse bin has sufficient size
		if(total_size >= size) {
			vector<pair<string,int>> pair_vec = {}; 
			int temp_counter = 0;
			for(int i = 0; i < garbage_registers.size(); i++) {
				string reg_name = garbage_registers[i]->get_name();
				int num_qubits = garbage_registers[i]->get_num_qubits();
				for(int j = 0; j < num_qubits; j++) {
					if(temp_counter < num_qubits) pair_vec.push_back({reg_name, j});
					else break;
					temp_counter++;
	
				}

				if(temp_counter > num_qubits) break;

			}

			QuantumVariable* qvar_recycled = new QuantumVariable("recycled_var",pair_vec);
			qvar_recycled->set_qreg("recycled_var");
			qvar_recycled->set_num_qubits(size);
			table->push_quantum_var(qvar_recycled);
			return qvar_recycled;

		}

		return nullptr;

	}



};

Garbage* Garbage::garbage = NULL;
