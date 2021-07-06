#ifndef COMPARISON_OPS_H
#define COMPARISON_OPS_H

#include "../circuit/Circuit.h"




void OR(Circuit& qc, string qreg1, int qubit1, string qreg2, int qubit2, string output, int output_qubit) {
	qc.ccx(qreg1, qubit1, qreg2, qubit2, output, output_qubit);
	qc.cx(qreg1, qubit1, output, output_qubit);
	qc.cx(qreg2, qubit2, output, output_qubit);
}


//GREATER THAN
void bool_greater_than(Circuit& qc, string qreg1, int qubit1, string qreg2, int qubit2, string output, int output_qubit) {
	//bitwise greater than circuit (>)
	qc.x(qreg2, qubit2);
	qc.ccx(qreg1, qubit1, qreg2, qubit2, output, output_qubit);
	qc.x(qreg2, qubit2);
}



void greater_than(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output, QuantumVariable ancillary_register) {
	Circuit::get_circuit()->add_qreg_list(&ancillary_register);

		QuantumVariable min = qvar1.get_num_qubits() <= qvar2.get_num_qubits() ? qvar1 : qvar2;
		QuantumVariable max = qvar1.get_num_qubits() >= qvar2.get_num_qubits() ? qvar1 : qvar2;

		if (qvar1.get_num_qubits() == qvar2.get_num_qubits()) { max = qvar1; min = qvar2; }

		unsigned int difference = abs(qvar1.get_num_qubits() - qvar2.get_num_qubits());


		//loop over every pair of qubits
		for (int ancilla = 0, current_qubit = 0; ancilla < ancillary_register.get_num_qubits(); ancilla++, current_qubit++) {

			if (current_qubit < difference) {
				if (ancilla == 0) {

					max.get_num_qubits() == qvar1.get_num_qubits() ? qc.cx(max.get_qreg(), current_qubit, output.get_qreg(), 0) : void();


					//whether or not move to next qubit pair
					qc.x(max.get_qreg(), current_qubit);
					qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.x(max.get_qreg(), current_qubit);



				}

				else {
					max.get_num_qubits() == qvar1.get_num_qubits() ? qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, output.get_qreg(), 0) : void();

					qc.x(max.get_qreg(), current_qubit);
					qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.x(max.get_qreg(), current_qubit);

				}


			}



			else {

				if (ancilla == 0) {
					bool_greater_than(qc, qvar1.get_qreg(), current_qubit, qvar2.get_qreg(), current_qubit, output.get_qreg(), 0);

					qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.cx(min.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.x(ancillary_register.get_qreg(), ancilla);

				}

				else {
					int state1 = qvar1.get_num_qubits() < qvar2.get_num_qubits() ? 1 : 0;
					int state2 = qvar1.get_num_qubits() > qvar2.get_num_qubits() ? 1 : 0;

					bool_greater_than(qc, qvar1.get_qreg(), current_qubit - (difference * state1), qvar2.get_qreg(), current_qubit - (difference * state2), ancillary_register.get_qreg(), ancilla);
					qc.ccx(ancillary_register.get_qreg(), ancilla - 1, ancillary_register.get_qreg(), ancilla, output.get_qreg(), 0);
					bool_greater_than(qc, qvar1.get_qreg(), current_qubit - (difference * state1), qvar2.get_qreg(), current_qubit - (difference * state2), ancillary_register.get_qreg(), ancilla);


					//calculate whether or not to move on to next qubit pair
					if (ancilla != ancillary_register.get_num_qubits() - 1) {
						qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
						qc.ccx(ancillary_register.get_qreg(), ancilla - 1, min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
						qc.cx(ancillary_register.get_qreg(), ancilla - 1, ancilla);
					}
				}
			}
		}


		//uncomputation start
		for (int current_qubit = ancillary_register.get_num_qubits() - 1, ancilla = ancillary_register.get_num_qubits() - 1; current_qubit > -1; current_qubit--, ancilla--) {

			if (current_qubit < difference) {
				if (current_qubit == 0) {

					qc.x(max.get_qreg(), current_qubit);
					qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.x(max.get_qreg(), current_qubit);

				}

				else {

					qc.x(max.get_qreg(), current_qubit);
					qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.x(max.get_qreg(), current_qubit);

				}
			}

			else {
				if (current_qubit == 0) {
					qc.x(ancillary_register.get_qreg(), ancilla);
					qc.cx(min.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				}

				else if (current_qubit != ancillary_register.get_num_qubits() - 1) {
					qc.cx(ancillary_register.get_qreg(), ancilla - 1, ancilla);
					qc.ccx(ancillary_register.get_qreg(), ancilla - 1, min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
					qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				}
			}
		}
		//uncomputation end

	
}



//LESS THAN
void bool_less_than(Circuit& qc, string qreg1, int qubit1, string qreg2, int qubit2, string output, int output_qubit) {
	//bitwise less than circuit (<)
	qc.x(qreg1, qubit1);
	qc.ccx(qreg1, qubit1, qreg2, qubit2, output, output_qubit);
	qc.x(qreg1, qubit1);

}



void less_than(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output, QuantumVariable ancillary_register) {
	Circuit::get_circuit()->add_qreg_list(&ancillary_register);

	QuantumVariable min = qvar1.get_num_qubits() <= qvar2.get_num_qubits() ? qvar1 : qvar2;
	QuantumVariable max = qvar1.get_num_qubits() >= qvar2.get_num_qubits() ? qvar1 : qvar2;
	if (qvar1.get_num_qubits() == qvar2.get_num_qubits()) { max = qvar1; min = qvar2; }

	unsigned int difference = abs(qvar1.get_num_qubits() - qvar2.get_num_qubits());


	//loop over every pair of qubits
	for (int ancilla = 0, current_qubit = 0; ancilla < ancillary_register.get_num_qubits(); ancilla++, current_qubit++) {

		if (current_qubit < difference) {
			if (ancilla == 0) {

				max.get_num_qubits() == qvar2.get_num_qubits() ? qc.cx(max.get_qreg(), current_qubit, output.get_qreg(), 0) : void();


				//whether or not move to next qubit pair
				qc.x(max.get_qreg(), current_qubit);
				qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), current_qubit);



			}

			else {
				max.get_num_qubits() == qvar2.get_num_qubits() ? qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, output.get_qreg(), 0) : void();

				qc.x(max.get_qreg(), current_qubit);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), current_qubit);

			}


		}



		else {

			if (ancilla == 0) {
				bool_less_than(qc, qvar1.get_qreg(), current_qubit, qvar2.get_qreg(), current_qubit, output.get_qreg(), 0);

				qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.cx(min.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.x(ancillary_register.get_qreg(), ancilla);

			}

			else {

				//helps change current qubit offset so that size differences in registers are accounted for
				int state1 = qvar1.get_num_qubits() < qvar2.get_num_qubits() ? 1 : 0;
				int state2 = qvar1.get_num_qubits() > qvar2.get_num_qubits() ? 1 : 0;

				//compute less than for qubit pair
				bool_less_than(qc, qvar1.get_qreg(), current_qubit - (difference * state1), qvar2.get_qreg(), current_qubit - (difference * state2), ancillary_register.get_qreg(), ancilla);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, ancillary_register.get_qreg(), ancilla, output.get_qreg(), 0);
				bool_less_than(qc, qvar1.get_qreg(), current_qubit - (difference * state1), qvar2.get_qreg(), current_qubit - (difference * state2), ancillary_register.get_qreg(), ancilla);


				//calculate whether or not to move on to next qubit pair
				if (ancilla != ancillary_register.get_num_qubits() - 1) {
					qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
					qc.ccx(ancillary_register.get_qreg(), ancilla - 1, min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
					qc.cx(ancillary_register.get_qreg(), ancilla - 1, ancilla);
				}
			}
		}
	}


	//uncomputation start
	for (int current_qubit = ancillary_register.get_num_qubits() - 1, ancilla = ancillary_register.get_num_qubits() - 1; current_qubit > -1; current_qubit--, ancilla--) {

		if (current_qubit < difference) {
			if (current_qubit == 0) {

				qc.x(max.get_qreg(), current_qubit);
				qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), current_qubit);

			}

			else {

				qc.x(max.get_qreg(), current_qubit);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), current_qubit);

			}
		}

		else {
			if (current_qubit == 0) {
				qc.x(ancillary_register.get_qreg(), ancilla);
				qc.cx(min.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
			}

			else if (current_qubit != ancillary_register.get_num_qubits() - 1 && current_qubit != 0) {
				qc.cx(ancillary_register.get_qreg(), ancilla - 1, ancilla);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
			}
		}
	}
	//uncomputation end
}




void circ_equal_to(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output, QuantumVariable ancillary_register) {
	Circuit::get_circuit()->add_qreg_list(&ancillary_register);

	QuantumVariable min = qvar1.get_num_qubits() <= qvar2.get_num_qubits() ? qvar1 : qvar2;
	QuantumVariable max = qvar1.get_num_qubits() >= qvar2.get_num_qubits() ? qvar1 : qvar2;
	if (qvar1.get_num_qubits() == qvar2.get_num_qubits()) { max = qvar1; min = qvar2; }

	unsigned int difference = abs(qvar1.get_num_qubits() - qvar2.get_num_qubits());
	//create ancillary register
	

	for (int current_qubit = 0, ancilla = 0; ancilla < ancillary_register.get_num_qubits(); ancilla++, current_qubit++) {

		if (current_qubit < difference) {
			if (ancilla == 0) {
				qc.x(max.get_qreg(), 0);
				qc.cx(max.get_qreg(), 0, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), 0);
			}

			else {
				qc.x(max.get_qreg(), current_qubit);
				qc.ccx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla - 1, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), current_qubit);


			}

		}

		else {

			if (ancilla == 0) {
				qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.cx(min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
				qc.x(ancillary_register.get_qreg(), ancilla);



			}


			else {
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
				qc.cx(ancillary_register.get_qreg(), ancilla - 1, ancilla);



			}
		}


	}


	qc.cx(ancillary_register.get_qreg(), ancillary_register.get_num_qubits() - 1, output.get_qreg(), 0);



	//uncomputation start
	for (int current_qubit = max.get_num_qubits() - 1, ancilla = ancillary_register.get_num_qubits() - 1; ancilla > -1; ancilla--, current_qubit--) {

		if (current_qubit < difference) {
			if (ancilla == 0) {
				qc.x(max.get_qreg(), 0);
				qc.cx(max.get_qreg(), 0, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), 0);
			}

			else {
				qc.x(max.get_qreg(), current_qubit);
				qc.ccx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla - 1, ancillary_register.get_qreg(), ancilla);
				qc.x(max.get_qreg(), current_qubit);


			}

		}

		else {

			if (ancilla == 0) {
				qc.x(ancillary_register.get_qreg(), ancilla);
				qc.cx(min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
				qc.cx(max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);



			}


			else {
				qc.cx(ancillary_register.get_qreg(), ancilla - 1, ancilla);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, min.get_qreg(), current_qubit - difference, ancillary_register.get_qreg(), ancilla);
				qc.ccx(ancillary_register.get_qreg(), ancilla - 1, max.get_qreg(), current_qubit, ancillary_register.get_qreg(), ancilla);

			}
		}


	}

	//uncomputation end



}



void greater_or_equal(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output, QuantumVariable ancillary_register) {
	Circuit::get_circuit()->add_qreg_list(&ancillary_register);
	QuantumVariable output1("output1");
	output1.set_num_qubits(1);
	qc.add_qregister(output1);


	QuantumVariable output2("output2");
	output2.set_num_qubits(1);
	qc.add_qregister(output2);

	greater_than(qc, qvar1, qvar2, output1, ancillary_register);

	circ_equal_to(qc, qvar1, qvar2, output2, ancillary_register);

	OR(qc, output1.get_qreg(), 0, output2.get_qreg(), 0, output.get_qreg(), 0);

	circ_equal_to(qc, qvar1, qvar2, output2, ancillary_register);

	greater_than(qc, qvar1, qvar2, output1, ancillary_register);



}



void less_or_equal(Circuit& qc, QuantumVariable qvar1, QuantumVariable qvar2, QuantumVariable output, QuantumVariable ancillary_register) {

	Circuit::get_circuit()->add_qreg_list(&ancillary_register);

	QuantumVariable output1("output1");
	output1.set_num_qubits(1);
	qc.add_qregister(output1);


	QuantumVariable output2("output2");
	output2.set_num_qubits(1);
	qc.add_qregister(output2);

	less_than(qc, qvar1, qvar2, output1, ancillary_register);

	circ_equal_to(qc, qvar1, qvar2, output2, ancillary_register);

	OR(qc, output1.get_qreg(), 0, output2.get_qreg(), 0, output.get_qreg(), 0);

}

#endif
