#pragma once
#include "../circuit/circuit.h"
#include <iostream>



void multi_ctrl_gate(Circuit& qc, void(Circuit::*gate)(string, unsigned int, string, unsigned int), QuantumVariable control, QuantumVariable target, QuantumVariable ancillary_register) {
	// control no. 2 -> initialize to second qubit	
	unsigned int c2 = 1;
	// we start from the first ancilla qubit
	unsigned int current_ancilla = 0; 
	
	// interate over each control qubit
	for (unsigned int c1 = 0; c1 < control.get_num_qubits(); c1++) {
		// start with applying the unique behaviour that comes from the first operation -> Toffoli from the 1st + 2nd control qubits
		// targeted at the first ancilla
		if (c1 == 0) {
			qc.ccx(control.get_qreg(), c1, control.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);
			c2 = 0;
			c1++;

		}

		// default behaviour -> start with applying Toffoli gates from each control, ancilla to each successive ancilla qubit
		else {
			qc.ccx(control.get_qreg(), c1, ancillary_register.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);
			c2++;
		}

		current_ancilla++;
	
	}

	current_ancilla--;

	// apply whatever quantum gate needs to be applied
	(qc.*gate)(ancillary_register.get_qreg(), current_ancilla, target.get_qreg(), 0);

	c2 = ancillary_register.get_num_qubits() - 2;

	// go in reverse and uncompute
	for (int c1 = control.get_num_qubits() - 1; c1 > -1; c1--) {
		
		qc.ccx(control.get_qreg(), c1, ancillary_register.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);

		// reverse first operation default behaviour
		if (c1 == 2) {
			c1 = 0;
			c2 = 1;
			qc.ccx(control.get_qreg(), c1, control.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla-1);
			break;
		}

		current_ancilla--;
		c2--;

	}
	


}


void multi_ctrl_gate(Circuit& qc, void(Circuit::* gate)(string, unsigned int, string, unsigned int, double), QuantumVariable control, QuantumVariable target, QuantumVariable ancillary_register, double theta) {

	unsigned int c2 = 1;
	unsigned int current_ancilla = 0;
	for (unsigned int c1 = 0; c1 < control.get_num_qubits(); c1++) {
		if (c1 == 0) {
			qc.ccx(control.get_qreg(), c1, control.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);
			c2 = 0;
			c1++;

		}

		else {
			qc.ccx(control.get_qreg(), c1, ancillary_register.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);
			c2++;
		}

		current_ancilla++;

	}

	current_ancilla--;

	(qc.*gate)(ancillary_register.get_qreg(), current_ancilla, target.get_qreg(), 0,theta);

	c2 = ancillary_register.get_num_qubits() - 2;

	for (int c1 = control.get_num_qubits() - 1; c1 > -1; c1--) {

		qc.ccx(control.get_qreg(), c1, ancillary_register.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);

		if (c1 == 2) {
			c1 = 0;
			c2 = 1;
			qc.ccx(control.get_qreg(), c1, control.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla - 1);
			break;
		}

		current_ancilla--;
		c2--;

	}



}


void multi_ctrl_gate(Circuit& qc, void(Circuit::* gate)(string, unsigned int, string, unsigned int, string), QuantumVariable control, QuantumVariable target, QuantumVariable ancillary_register, string theta) {

	unsigned int c2 = 1;
	unsigned int current_ancilla = 0;
	for (unsigned int c1 = 0; c1 < control.get_num_qubits(); c1++) {
		if (c1 == 0) {
			qc.ccx(control.get_qreg(), c1, control.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);
			c2 = 0;
			c1++;

		}

		else {
			qc.ccx(control.get_qreg(), c1, ancillary_register.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);
			c2++;
		}

		current_ancilla++;

	}

	current_ancilla--;

	(qc.*gate)(ancillary_register.get_qreg(), current_ancilla, target.get_qreg(), 0, theta);

	c2 = ancillary_register.get_num_qubits() - 2;

	for (int c1 = control.get_num_qubits() - 1; c1 > -1; c1--) {

		qc.ccx(control.get_qreg(), c1, ancillary_register.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);

		if (c1 == 2) {
			c1 = 0;
			c2 = 1;
			qc.ccx(control.get_qreg(), c1, control.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla - 1);
			break;
		}

		current_ancilla--;
		c2--;

	}



}


// diffusion operator
void AmplitudeAmplify(Circuit& qc, QuantumVariable qvar, QuantumVariable target, QuantumVariable ancilla) {
	if (qvar.get_num_qubits() == 1) {
		std::cerr << "SEARCH CANNOT BE PERFORMED ON SMALL SEARCH SPACES";
	}

	else {
		
		qc.h(qvar.get_qreg());
		qc.x(qvar.get_qreg());
		


		if (qvar.get_num_qubits() == 2) {
			qc.cz(qvar.get_qreg(), 0, 1);
		}

		else if (qvar.get_num_qubits() == 3) {
			qc.h(qvar.get_qreg(), 2);
			qc.ccx(qvar.get_qreg(), 0, 1, 2);
			qc.h(qvar.get_qreg(), 2);
		}


		else {
			multi_ctrl_gate(qc, &Circuit::cz, qvar, target, ancilla);
		}

		qc.x(qvar.get_qreg());
		qc.h(qvar.get_qreg());
		


	}
}


