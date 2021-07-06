#pragma once
#include "../circuit/Circuit.h"
#include <iostream>



void multiCtrlGate(Circuit& qc, void(Circuit::*gate)(string, unsigned int, string, unsigned int), QuantumVariable control, QuantumVariable target, QuantumVariable ancillary_register) {
	
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

	(qc.*gate)(ancillary_register.get_qreg(), current_ancilla, target.get_qreg(), 0);

	c2 = ancillary_register.get_num_qubits() - 2;

	for (int c1 = control.get_num_qubits() - 1; c1 > -1; c1--) {
		
		qc.ccx(control.get_qreg(), c1, ancillary_register.get_qreg(), c2, ancillary_register.get_qreg(), current_ancilla);

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


void multiCtrlGate(Circuit& qc, void(Circuit::* gate)(string, unsigned int, string, unsigned int, double), QuantumVariable control, QuantumVariable target, QuantumVariable ancillary_register, double theta) {

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


void multiCtrlGate(Circuit& qc, void(Circuit::* gate)(string, unsigned int, string, unsigned int, string), QuantumVariable control, QuantumVariable target, QuantumVariable ancillary_register, string theta) {

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
			multiCtrlGate(qc, &Circuit::cz, qvar, target, ancilla);
		}

		qc.x(qvar.get_qreg());
		qc.h(qvar.get_qreg());
		


	}
}


