#pragma once
#include <vector>
#include <string>
#include "../tables/data.h"
using namespace std;

class Garbage {
	 Garbage() {}
	 static Garbage* garbage;
	 vector<QuantumVariable*> garbage_registers;

public:
	static Garbage* get_garbage() {
		if (!garbage) {
			garbage = new Garbage();
		}

		return garbage;
	}

	void add_garbage(QuantumVariable* qvar) {
		garbage_registers.push_back(qvar);
	}

	QuantumVariable* get_garbage_register(int register_size) {
		
		for (int reg = 0; reg < garbage_registers.size(); reg++) {
			if (garbage_registers[reg]->get_num_qubits() == register_size) {
				return garbage_registers[reg];
			}
		}

		return NULL;
	}


};

Garbage* Garbage::garbage = NULL;
