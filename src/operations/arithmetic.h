#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "qft.h"
#include "../tables/data.h"
#include "../general/base_conversion.h"
#include <math.h>
#include <cassert>


//ADDITION UNIT

//FUNCTION ALLOWS ADDITION OF NUMBER AND QUANTUM REGISTER WITHOUT STORING NUMBER IN MEMORY


void apply_addition_phases(Circuit& qc, unsigned long long num, QuantumVariable output, bool is_sub = false, vector<Conditional> controls = {}) {
	// define pi

	// get number of qubits required to store number in binary
	int num_qubits = floor(log2(num)) + 1;

	// get  what padding qubits are required
	int padding = output.get_num_qubits() - num_qubits;

	// get bitstring of number
	string bitstring = to_binary(num, padding);

	// set total phase of number to zero
	long double phase = 0;

	// iterate over each bit in bitstring
	for (int bit = 0; bit < bitstring.size(); bit++) {
		// append values to phase. These values take the form of 1/(2^(n+1)) for all n from 0 to bitstring size
		string temp = "";
		temp += bitstring[bit];
		phase += stoi(temp) * (1 / pow(2, bit + 1));
		
	}

	// multiply by 2*pi
	phase *= pi * 2;

	// if we wish to subtract the number, multiply the phase by -1 to get the inverse
	if (is_sub) phase *= -1;

	// iterate over each qubit in the register we wish to apply phases to
	for (int j = 0; j < output.get_num_qubits(); j++) {
		// apply each phase so that it is twice the last

		// case for no-control addition phases
		if (controls.empty()) qc.u(output.get_qreg(), j, phase);

		// case for single-control addition phases
		else if (controls.size() == 1) qc.cu(controls[0].get_control(), 0, output.get_qreg(), j, phase);

		// case for double-control addition phases
		else if (controls.size() == 2) qc.ccu1(controls[0].get_control(), 0, controls[1].get_control(), 0, output.get_qreg(), j, phase);

		//  multiple-control (>2) addition phases are distilled into single-control phases through an ancillary qubit during compilation

		// double phase
		phase *= 2;
	}
}


// FUNCTION PERFORMS ADDITION ROTATIONS THROUGH FOURIER-TRANSFORM

void u_rotations_addition_append(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, bool inverse = false, int multiplier = 1, vector<Conditional> controls = {}) {
	// get the size difference between the two registers
	int difference = abs(qvar1.get_num_qubits() - qvar2.get_num_qubits());

	// iterate over each qubit in first register
	for (int i = 0; i < qvar1.get_num_qubits(); i++) {

		// set our pi_divisor (a value, x , which is going to apply a phase as 1/(2^x)
		int pi_divisor = 0;

		// copy the pi_divisor
		int pi_divisor_cpy = pi_divisor;

		// string which holds phase
		double long phase = pi;

		// if we wish to perform subtraction, negate the phase to get its inverse
		if (inverse) phase *= -1;

		
		// for each qubit in the first register, we wish to iterate over all qubits in second register, applying their cumulative phases
		for (int j = i; j < qvar2.get_num_qubits() + difference /*we assume that both registers are of the same size*/; j++) {
			// should the qubit index in the second register be within-bounds
			if(j > difference-1) {
				// case for no-control addition
				if (controls.empty()) qc.cu(qvar2.get_qreg(), j - difference, qvar1.get_qreg(), i, multiplier *  (phase/(pow(2, pi_divisor))));

				// case for single-control addition
				else if (controls.size() == 1) qc.ccu1(qvar2.get_qreg(), j - difference, qvar1.get_qreg(), i, controls[0].get_control(), 0, multiplier * (phase/(pow(2, pi_divisor))));
			}

			// increase our pi_divisor by 1
			pi_divisor++;
		}

		// restore pi_divisor to previous initial value and keep incrementing pi_divisor
		pi_divisor = pi_divisor_cpy + 1;
		
		// increment the copy of the pi_divisor by one
		pi_divisor_cpy++;
	}
}



// ADD APPEND - CORRESPONDS TO (+=)
// E.G. REG_1 += REG_2
void add_append(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, bool inverse = false, vector<Conditional> controls = {}) {
	// perform QFT
	QFT(qc, qvar1, false);
	// apply addition append phases
	u_rotations_addition_append(qc, qvar1, qvar2, false, 1,controls);
	// perform Inverse QFT
	InvQFT(qc, qvar1, false);
}



//ADD APPEND - CORRESPONDS TO (+=)
// E.G. REG += INT
void add_append(Circuit& qc, QuantumVariable qvar, unsigned long long number, bool inverse = false, vector<Conditional> controls = {}) {
	// apply QFT
	QFT(qc, qvar, false);
	// apply addition phases corresponding to integer to itself
	apply_addition_phases(qc, number, qvar, false, controls);
	// perform Inverse QFT
	InvQFT(qc, qvar, false);
}


// ADDITION - CORRESPONDS TO (+)
// E.G. REG_1 + REG_2
void add(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output, bool inverse = false, vector<Conditional> controls = {}) {
	// apply InvQFT if we wish to perform inverse addition - this is done so that a scratch register can be uncomputed
	if (!inverse) qc.h(output.get_qreg());

	// else apply QFT for standard |0> state
	else QFT(qc, output, false);

	// apply RESULT += A
	u_rotations_addition_append(qc, output, qvar1, inverse, 1, controls);

	// apply RESULT += B
	u_rotations_addition_append(qc, output, qvar2, inverse, 1, controls);

	// the above corresponds to RESULT += (A+B) which if RESULT is |0> is (A+B)

	// apply InvQFT to obtain state in computational basis
	InvQFT(qc, output, false);

}


// ADDITION - CORRESPONDS TO (+)
// E.G. REG + INT
void add(Circuit& qc, QuantumVariable qvar, unsigned long long num, QuantumVariable output, bool inverse = false, vector<Conditional> controls = {}) {
	// apply InvQFT if we wish to perform inverse addition - this is done so that a scratch register can be uncomputed
	if (!inverse) qc.h(output.get_qreg());

	// else apply QFT for standard |0> state
	else QFT(qc, output, false);

	// apply RESULT += A
	u_rotations_addition_append(qc, output, qvar, inverse, 1, controls);
	
	// apply RESULT += INT
	apply_addition_phases(qc, num, output, inverse, controls);

	// apply InvQFT
	InvQFT(qc, output, false);
	
}






//SUBTRACTION UNIT

void subtract(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output, vector<Conditional> controls = {}) {
	qc.h(output.get_qreg());

	u_rotations_addition_append(qc, output, qvar1, false, 1, controls);
	u_rotations_addition_append(qc,output, qvar2,true, 1, controls);

	InvQFT(qc, output, false);
}




void subtract(Circuit& qc, QuantumVariable qvar, unsigned long long num, QuantumVariable output, vector<Conditional> controls = {}) {
	qc.h(output.get_qreg());

	u_rotations_addition_append(qc, output, qvar, false, 1, controls);

	apply_addition_phases(qc, num, output, true, controls);
	

	InvQFT(qc, output, false);
}





void subtract_append(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, vector<Conditional> controls = {}) {
	QFT(qc, qvar1, false);
	u_rotations_addition_append(qc, qvar1, qvar2, true, 1, controls);
	InvQFT(qc, qvar1, false);
}



void subtract_append(Circuit& qc, QuantumVariable qvar, unsigned long long number, vector<Conditional> controls = {}) {
	QFT(qc, qvar, false);
	apply_addition_phases(qc, number, qvar, true, controls);
	InvQFT(qc, qvar, false);
}



//MULTIPLICATION UNIT

// this function is used to multiply a register by a number
void multiply_by_int(Circuit& qc, QuantumVariable qvar1, unsigned int number, QuantumVariable output, bool inverse = false, vector<Conditional> controls = {}) {
	
	// if not inverse, apply QFT
	if (!inverse) qc.h(output.get_qreg());
	else QFT(qc, output, false);
	
	// apply multiplication phases through repeated addition
	u_rotations_addition_append(qc, output, qvar1, inverse, number, controls);
	InvQFT(qc, output, false);
	
	
}



// this function multiplies a quantum register by a number and stores it within the same register, ie it performs the operation: REG *= NUM
void multiply_append_by_int(Circuit& qc, QuantumVariable qvar, int number, bool is_inverse = false, vector<Conditional> controls = {}) {
	
	// apply QFT
	QFT(qc, qvar, false);
	
	// apply multiplication phases through repeated addition
	u_rotations_addition_append(qc, qvar, qvar, is_inverse, number, controls);
	
	// apply InvQFT
	InvQFT(qc, qvar, false);
	

}






void multiplication_rotations(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, string qreg_output, unsigned int output_qubit, int power) {
	unsigned int difference = abs(qvar1.get_num_qubits() - qvar2.get_num_qubits());
	int power_of_two = 2 + difference;
	for (int index1 = 0; index1 < qvar1.get_num_qubits(); index1++) {
		int current_power = power_of_two;
		for (int index2 = 0; index2 < qvar2.get_num_qubits(); index2++) {
			qc.ccu1(qvar1.get_qreg(), index1, qvar2.get_qreg(), index2, qreg_output, output_qubit, 2*pi* (power/pow(2, current_power)));
			current_power++;
		}
		
		power_of_two++;
	}

}



// this function is used to multiply two quantum variables
void multiply_reg_by_reg(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output) {

	// initialise the output register
	for (int i = 0; i < output.get_num_qubits(); i++) {
		qc.h(output.get_qreg(), i);
	}


	// perform the multiplication
	int num_iterations = 1;
	// iterate through each qubit of the output register
	for (int output_qubit = 0; output_qubit < output.get_num_qubits(); output_qubit++) {
		
		// apply the multiplication rotations
		multiplication_rotations(qc, qvar1, qvar2, output.get_qreg(), output_qubit, num_iterations);


		num_iterations *= 2;
	}
	
	// apply the inverse QFT
	InvQFT(qc, output,  false);

}


// this function is used to calculate the modulo of a quantum variable with a positive integer that is a power 
// of two

void modulo_by_two_exp(Circuit& qc, QuantumVariable& qvar, unsigned long long num,  QuantumVariable& output) {
	assert(log2(num) - (int)log2(num) == 0);
	int exp = log2(num);

	for(int i = 0; i < exp; i++) {
		qc.cx(qvar.get_qreg(), qvar.get_num_qubits() - (i+1), output.get_qreg(), i);
	}

}


#endif
