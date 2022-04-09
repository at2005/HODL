#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <fstream>
#include <iostream>
#include <vector>
#include "gates.h"
#include "../tables/data.h"


using namespace std;


class Circuit {
	Circuit(string target = "out.qasm");
	Circuit(Circuit& circ);
public:
	
	static Circuit* get_circuit(string target="out.qasm") { 
		if (!circuit) circuit = new Circuit(target);
		return circuit; 
	}

	void add_qreg_list(QuantumVariable* qvar) {
		for (int i = 0; i < qregs.size(); i++) {
			if (qregs[i]->get_qreg() == qvar->get_qreg()) {
				return;
			}
		}

		this->qregs.push_back(qvar);
	}

	vector <QuantumVariable*> get_qvars() {
		return this->qregs;
	}

	const int get_num_qubits();
	void inc_num_qubits();
	//ID GATE FOR SINGLE QUBIT

	const void id(string qreg, unsigned int qubit_index);
	//ID GATE FOR MULTIPLE QUBITS

	const void id(string qreg);

	const void measure_all() {
		for (int i = 0; i < qubits; i++) {
			outputQASM << "measure qr[" << i << "] -> cr[" << i << "];\n";
		}
	}

	const void measure(int qreg, int creg) {
		outputQASM << "measure qr[" << qreg << "] -> cr[" << creg << "];\n";
	}

	const void measure(string qreg, string creg) {
		outputQASM << "measure " << qreg << " -> " << creg << ";\n";
	}

	void add_qregister(QuantumVariable& qvar);
	void add_cregister(string creg, int measurable_qubits);

	~Circuit();
	//HADAMARD GATE FOR SINGLE QUBIT
	const void h(string qreg, unsigned int qubit_index);
	//HADAMARD GATE FOR MULTIPLE QUBITS
	const void h(string qreg);


	//X GATE FOR SINGLE QUBIT
	const void x(string qreg, unsigned int qubit_index);
	//X GATE FOR MULTIPLE QUBITS
	const void x(string qreg);


	//Y GATE FOR SINGLE QUBIT
	const void y(string qreg, unsigned int qubit_index);
	//Y GATE FOR MULTIPLE QUBITS
	const void y(string qreg);

	//Z GATE FOR SINGLE QUBIT
	const void z(string qreg, unsigned int qubit_index);
	//Z GATE FOR MULTIPLE QUBITS
	const void z(string qreg);


	//S GATE FOR SINGLE QUBIT
	const void s(string qreg,unsigned int qubit_index);
	//S GATE FOR MULTIPLE QUBITS
	const void s(string qreg);

	//SDG GATE FOR SINGLE QUBIT
	const void sdg(string qreg, unsigned int qubit_index);
	//SDG GATE FOR MULTIPLE QUBITS
	const void sdg(string qreg);

	//T GATE FOR SINGLE QUBIT
	const void t(string qreg, unsigned int qubit_index);
	//T GATE FOR MULTIPLE QUBITS
	const void t(string qreg);

	//ROTATE X GATE FOR SINGLE QUBIT
	const void rx(string qreg, unsigned int qubit_index, double angle_in_radians);
	//ROTATE X GATE FOR MULTIPLE QUBITS
	const void rx(string qreg,double angle_in_radians);


	const void ry(string qreg,unsigned int qubit_index, double angle_in_radians);

	//ROTATE Y GATE FOR MULTIPLE QUBITS
	const void ry(string qreg, double angle_in_radians);


	//ROTATE Z GATE FOR SINGLE QUBIT
	const void rz(string qreg, unsigned int qubit_index, double angle_in_radians);

	//ROTATE Z GATE FOR MULTIPLE QUBITS
	const void rz(string qreg, double angle_in_radians);


	//UNITARY OPERATION CORRESPONDING TO THE FOLLOWING MATRIX REPRESENTATION: M00 -> 1, M01 -> 0, M10 -> 0, M11 -> e^2*pi*theta (theta = angle_in_radians)
	

	//for part of quantum register with angle of type double
	const void u(string qreg,unsigned int qubit_index, double angle_in_radians);

	//for entire quantum register with angle of type double
	const void u(string qreg, double angle_in_radians);


	//for part of quantum register
	const void u(string qreg,unsigned int qubit_index, string theta);

	//for entire quantum register
	const void u(string qreg, string theta);




	//CONTROLLED UNITARY OPERATION

	//cu for part of quantum register
	const void cu(string qreg,unsigned int control, unsigned int target, double angle_in_radians);
	const void cu(string qreg1, unsigned int control, string qreg2, unsigned int target, double angle_in_radians);


	//cu for fourier transform
	//cu for fourier transform of part of quantum register
	const void cu(string qreg,unsigned int control, unsigned int target, string init, unsigned long long int arg);
	const void cu(string qreg1, unsigned int control, string qreg2, unsigned int target, string init, unsigned long long int arg);
	
	//cu for fourier transform
	const void cu(string qreg, unsigned int control, unsigned int target, string theta);
	const void cu(string qreg1, unsigned int control, string qreg2, unsigned int target, string theta);





	//CONTROLLED-X GATE FOR SINGLE PAIR OF QUBITS
	void cx(string qreg,unsigned int control, unsigned int target);
	void cx(string qreg1,unsigned int control,string qreg2, unsigned int target);
	void cx(string qreg1, string qreg2);
	

	//CONTROLLED-Z GATE FOR SINGLE PAIR OF QUBITS
	void cz(string qreg,unsigned int control, unsigned int target);
	void cz(string qreg1,unsigned int control,string qreg2, unsigned int target);
	void cz(string qreg1, string qreg2);


	//CONTROLLED-HADAMARD GATE FOR SINGLE PAIR OF QUBITS
	void ch(string qreg,unsigned int control, unsigned int target);
	void ch(string qreg1,unsigned int control, string qreg2,unsigned int target);


	//TOFFOLI GATE FOR SINGLE TRIPLET OF QUBITS

	void ccx(string qreg,unsigned int control1, unsigned int control2, unsigned int target);
	void ccx(string qreg1, unsigned int control1, string qreg2, unsigned int control2,string qreg3, unsigned int target);

	//MULTIPLE-CONTROLLED CCU1 GATE
	void ccu1(string qreg,unsigned int control1, unsigned int control2, unsigned int target, string theta);
	void ccu1(string qreg1, unsigned int control1,string qreg2, unsigned int control2,string qreg3, unsigned int target, string theta);

	void ccu1(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, double theta);



	//MULTIPLE-CONTROLLED CC-GATE
	void ccgate(string qreg, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int control3, int (*gate)(string qreg_func1, unsigned int qubit1, string qreg_func2, unsigned int qubit2));
	void ccgate(string qreg, unsigned int control1, unsigned int control2, unsigned int target, int (*gate)(string qreg_func, unsigned int qubit1, unsigned int qubit2));
	void ccgate(string qreg, unsigned int control1, unsigned int control2, unsigned int target, string theta, int (*gate)(string qreg_func, unsigned int qubit1, unsigned int qubit2, string theta_func));
	void ccgate(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, string theta, int (*gate)(string qreg_func1, unsigned int qubit1, string qreg_func2, unsigned int qubit2, string theta));




	//SWAP GATE FOR SINGLE PAIR OF QUBITS
	void swap(string qreg,unsigned int index1, unsigned int index2);
	void swap(string qreg1,unsigned int index1, string qreg2,unsigned int index2);
	void swap(string qreg1, string qreg2);

	const void barrier();
	const void barrier(string qreg);
	
	unsigned int total_qubits;

private:
	static Circuit* circuit;
	unsigned int qubits;
	unsigned int num_qubits_measured;
	string system;
	ofstream outputQASM;
	vector<QuantumVariable*> qregs;
	vector<string> current_qregs;
	int total_registers;
};

#endif
