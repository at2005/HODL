#ifndef QFT_H
#define QFT_H

#include "../circuit/circuit.h"
#include "../compiler/conditionals.h"
#include <math.h>
//implement an n qubit Quantum Fourier Transform and its inverse


const double pi = 3.14159265358979323846;

void QFT(Circuit& qc, QuantumVariable qvar1, bool do_swaps = true) {

	for (int i = 0; i < qvar1.get_num_qubits(); i++) {
		if (i == qvar1.get_num_qubits() - 1) {
			qc.h(qvar1.get_qreg(), i);
			break;
		}

		else {
			qc.h(qvar1.get_qreg(), i);
			unsigned int counter = 1;

			for (int j = i + 1; j < qvar1.get_num_qubits(); j++) {
				if (j == i) {
					break;
				}
				
					qc.cu(qvar1.get_qreg(), j, i, pi /  pow(2, counter));
				
				counter++;
			}

			


		}
	}

	if (do_swaps) {
//		swapQFT(qc, 0, qvar1.get_num_qubits());
	}
}


void QFT(Circuit& qc, QuantumVariable qvar1, vector<Conditional> controls, bool do_swaps = true) {


	for (int i = 0; i < qvar1.get_num_qubits(); i++) {
		if (i == qvar1.get_num_qubits() - 1) {
			if (controls.size() == 1) {
				qc.ch(controls[0].get_control(), 0 , qvar1.get_qreg(), i);
			}

			break;
		}

		else {
			if (controls.size() == 1) {
				qc.ch(controls[0].get_control(), 0, qvar1.get_qreg(), i);
			}
	
			unsigned int counter = 1;

			for (int j = i + 1; j < qvar1.get_num_qubits(); j++) {
				if (j == i) {
					break;
				}

				if (controls.size() == 1) {
					qc.ccu1(controls[0].get_control(), 0 ,qvar1.get_qreg(), j, qvar1.get_qreg(), i, pi/(pow(2, counter)));
				}
				counter++;
			}

			


		}
	}

	if (do_swaps) {
//		swapQFT(qc, 0, qvar1.get_num_qubits());
	}
}


// partial QFT
void QFT(Circuit& qc, QuantumVariable qvar1, int start, int end, bool do_swaps = true) {


	for (int i = start; i < end; i++) {
		if (i == end - 1) {
			qc.h(qvar1.get_qreg(), i);
			break;
		}

		else {
			qc.h(qvar1.get_qreg(), i);
			unsigned int counter = 1;

			for (int j = i + 1; j < qvar1.get_num_qubits(); j++) {
				if (j == i) {
					break;
				}
				qc.cu(qvar1.get_qreg(), j, i, pi/pow(2, counter));
				counter++;
			}

			


		}
	}

	if (do_swaps) {
	//	swapQFT(qc, start, end);
	}
}





void qft_multiplication(Circuit& qc, QuantumVariable qvar, int number) {
	
}






void InvQFT(Circuit& qc, QuantumVariable qvar, bool do_swaps = true) {
	if (do_swaps) {
	//	swapQFT(qc, 0, qvar.get_num_qubits());
	}

	int counter = 2;
	for (int i = qvar.get_num_qubits() - 1; i > -1; i--) {
		if (i == -1) {
			break;
		}

		else {
			int temp = counter;
			for (int j = qvar.get_num_qubits() - 1; j > i; j--) {
				if (i == j) {
					break;
				}
				qc.cu(qvar.get_qreg(), j, qvar.get_qreg(), i, -1*pi/pow(2, temp - 2));
				temp--;
			}

			qc.h(qvar.get_qreg(), i);
			
			counter++;
		}
	}

}

void InvQFT(Circuit& qc, QuantumVariable qvar, vector<Conditional> controls, bool do_swaps = true) {
	if (do_swaps) {
	//	swapQFT(qc, 0, qvar.get_num_qubits());
	}

	int counter = 2;
	for (int i = qvar.get_num_qubits() - 1; i > -1; i--) {
		if (i == -1) {
			break;
		}

		else {
			int temp = counter;
			for (int j = qvar.get_num_qubits() - 1; j > i; j--) {
				if (i == j) {
					break;
				}

				if (controls.size() == 1) {
					qc.ccu1(controls[0].get_control(), 0, qvar.get_qreg(), j, qvar.get_qreg(), i, -1*pi/(pow(2, temp - 2)));
				}
				temp--;
			}

			if (controls.size() == 1) {
				qc.ch(controls[0].get_control(), 0 , qvar.get_qreg(), i);
			}
			counter++;
		}
	}

}


// partial qft_dg
void InvQFT(Circuit& qc, QuantumVariable qvar, int start, int end, bool do_swaps = true) {
	if (do_swaps) {
		//swapQFT(qc, start, end);
	}

	int counter = 2;
	for (int i = end - 1; i > start-1; i--) {
		if (i == start-1) {
			break;
		}

		else {
			int temp = counter;
			for (int j = end - 1; j > i; j--) {
				if (i == j) {
					break;
				}
				qc.cu(qvar.get_qreg(), j, qvar.get_qreg(), i, -1*pi/ pow(2, temp - 2));
				temp--;
			}
			//qc.barrier();

			qc.h(qvar.get_qreg(), i);
			//qc.barrier();
			counter++;
		}
	}

}

#endif
