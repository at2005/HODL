#ifndef COMPILE_INSTR_H
#define COMPILE_INSTR_H

#include "instructions.h"
#include "../tables/symtab.h"
#include "../operations/arithmetic.h"
#include "../operations/grover.h"
#include "../lexer/tokens.h"
#include "../operations/comparison_operators.h"
#include <unordered_map>
#include "estimation_funcs.h"
#include "garbage.h"


QuantumVariable* alloc_ancilla(Circuit& qc, unsigned int size, SymbolTable* table) {

	QuantumVariable* ancilla = Garbage::get_garbage()->get_recycled(size, table);
	if(ancilla == nullptr) {
		// create ancillary register
		table->ADD_ANCILLA_REGISTER(size);
		// get ancillary register
		ancilla = table->search_qtable(table->GET_ANCILLA_REGISTER());				
	
	}

	qc.add_qregister(*ancilla);
	return ancilla;

}


void bitwise_or(Circuit& qc, QuantumVariable* qreg1, unsigned int index1, QuantumVariable* qreg2, unsigned int index2, QuantumVariable* result, unsigned int result_index) {
	qc.ccx(qreg1->get_qreg(), index1, qreg2->get_qreg(), index2, result->get_qreg(), result_index);
	qc.cx(qreg1->get_qreg(), index1, result->get_qreg(), result_index);
	qc.cx(qreg2->get_qreg(), index2, result->get_qreg(), result_index);

}


// FUNCTIONS TO PARSE INSTRUCTION TAPE AND COMPILE THE INSTRUCTIONS LISTED ON EACH CELL

// helper function to compile assembly-level instructions
void create_assembly(Circuit& qc, INSTRUCTION assembly) {
	try {
		// get the assembly function name
		string type = assembly.GET_PF().get_func_name();

		// get the function parameters
		vector<string> func_params = assembly.GET_PF().get_parameters();

		// if there are no parameters, throw an error
		if (func_params.size() == 0) throw "INCORRECT_PARAMS_ERROR";

		// case for HADAMARD Gate
		if (type == "H") {
			qc.h(func_params[0]);
		

		}

		// case for X/NOT Gate
		else if (type == "X") {
			qc.x(func_params[0]);
		}

		// case for Y Gate
		else if (type == "Y") {
			qc.y(func_params[0]);
		}

		// case for Z Gate
		else if (type == "Z") {
			qc.z(func_params[0]);
		}

		// case for CX Gate
		else if (type == "CX") {
			qc.cx(func_params[0], func_params[1]);
		}

		// case for CZ Gate
		else if (type == "CZ") {
			qc.cz(func_params[0], func_params[1]);
		}

		// case for S Gate
		else if (type == "S") {
			qc.s(func_params[0]);
		}

		// case for T Gate
		else if (type == "T") {
			qc.s(func_params[0]);
		}

		// case for TOFFOLI/CCX Gate
		else if (type == "CCX") {
			qc.ccx(func_params[0],0, func_params[1],0, func_params[2],0);
		}

		
	}

	// catch incorrect parameters exception
	catch (const char*) {
		// print error message
		cerr << "INCORRECT_PARAMS_ERROR";
		// exit program
		exit(1);
	}

	
}


static const unordered_map<string, string> op_map = {
	{"+", "+="},
	{"-", "-="},
	{"*", "*="}
};


// function for compiling the instruction tape
void compile_instructions(Circuit& qc, vector<INSTRUCTION> instructions, SymbolTable* table) {
	
	// iterator holds beginning of instruction tape
	vector<INSTRUCTION>::iterator instruction = instructions.begin();

	for (INSTRUCTION& instruction : instructions) {
		
		// switch symbol table/scope
		if (instruction.get_value() == "BEGIN") {
			table = instruction.get_table();
		}

		// switch symbol table back to original table
		else if(instruction.get_value() == "END") {
			SymbolTable* to_delete = table;
			table = table->get_parent_table();
			delete to_delete;
		}

		// if the instruction cell refers to an expression or operation
		else if (instruction.get_value() == "OPERATION") {
			
			// get operator type
			string type = instruction.GET_OP().get_type();
			
			// get left operand
			string reg0 = instruction.GET_OP().get_reg0();

			// get right operand
			string reg1 = instruction.GET_OP().get_reg1();
			
			// get result register in which we shall store our output
			string result = instruction.GET_OP().get_result();
			
			// look up result register in symbol table
			QuantumVariable* result_reg = table->search_qtable(result);
			
			// check if x= operator
			if ((reg0 == result or reg1 == result) and op_map.find(type) != op_map.end()) {
				type = op_map.find(type)->second;
			}
				
			

			// if both operands are identifiers
			if (isIdentifier(reg0) && isIdentifier(reg1)) {
				// look up left operand in symbol table
				QuantumVariable* qvar1 = table->search_qtable(reg0);
				
				// look up right operand in symbol table
				QuantumVariable* qvar2 = table->search_qtable(reg1);
				
				// compile addition
				if (type == "+") {
					add(qc, *qvar1, *qvar2, *result_reg, instruction.is_inverted(),instruction.get_controls());
					
				}

				// compile subtraction
				else if (type == "-") {
					subtract(qc, *qvar1, *qvar2, *result_reg, instruction.get_controls());
					
				}
				// compile multiplication
				else if (type == "*") {
					multiply_reg_by_reg(qc, *qvar1, *qvar2, *result_reg);
				}


				// compile addition-append
				else if (type == "+=") {
					add_append(qc, *qvar1, *qvar2, instruction.is_inverted(),instruction.get_controls());
					
				}

				// compile subtraction-append
				else if (type == "-=") {
					subtract_append(qc, *qvar1, *qvar2, instruction.get_controls());
					
				}

			

				// check if our operation is relational (comparison)
				if (isCompOp(type)) {
					// get size of ancillary register. It is the size of the largest input
					int num_qubits_ancilla = qvar1->get_num_qubits() > qvar2->get_num_qubits() ? qvar1->get_num_qubits() : qvar2->get_num_qubits();
				

					QuantumVariable* ancilla = Garbage::get_garbage()->get_recycled(num_qubits_ancilla, table);
					if(ancilla == nullptr) {
						// create ancillary register
						table->ADD_ANCILLA_REGISTER(num_qubits_ancilla);
						// get ancillary register
						ancilla = table->search_qtable(table->GET_ANCILLA_REGISTER());				
					
					}

					qc.add_qregister(*ancilla);
					

				/*	
					// reuse garbage register
					QuantumVariable* ancilla = Garbage::get_garbage()->get_garbage_register(num_qubits_ancilla);

					if(ancilla == nullptr) {
						// create ancillary register
						table->ADD_ANCILLA_REGISTER(num_qubits_ancilla);
						// get ancillary register
						QuantumVariable* ancilla = table->search_qtable(table->GET_ANCILLA_REGISTER());
					}

*/

					// add ancillary register to circuit
					
					// compile equality operator
					if (type == "==") {
						circ_equal_to(qc, *qvar1, *qvar2, *result_reg, *ancilla);
					}

					else if(type == "!=") {
						circ_equal_to(qc, *qvar1, *qvar2, *result_reg, *ancilla);
						qc.x(result_reg->get_qreg());
					}

					// compile greater-than operator
					else if (type == ">") {
						greater_than(qc, *qvar1, *qvar2, *result_reg, *ancilla);
					}

					// compile greater-than-or-equal-to operator
					else if (type == ">=") {
						greater_or_equal(qc, *qvar1, *qvar2, *result_reg, *ancilla);
					}

					// compile less-than operator
					else if (type == "<") {
						
						less_than(qc, *qvar1, *qvar2, *result_reg, *ancilla);
					}

					// compile less-than-or-equal-to operator
					else if (type == "<=") {
					
						less_or_equal(qc, *qvar1, *qvar2, *result_reg, *ancilla);
					}

					
					// add ancillary register to recycling bin
					Garbage::get_garbage()->add_garbage(table->search_qtable(table->GET_ANCILLA_REGISTER()));

				}

				// compile AND operator
				else if (type == "&") {
					qc.ccx(qvar1->get_qreg(), 0, qvar2->get_qreg(), 0, result_reg->get_qreg(), 0);
				}

				// compile OR operator
				else if (type == "|") {
					if(qvar1->get_num_qubits() > 1 || qvar2->get_num_qubits() > 1) {
						// n qubit OR gate, loop through all qubit pairs, eval or on ancilla, then X all of them, multi-controlled gate
						
						QuantumVariable* max_reg_or = qvar1;
						// get min and max qubits
						unsigned int max_qubits_or = qvar1->get_num_qubits();
						unsigned int min_qubits_or = qvar2->get_num_qubits();
						if(max_qubits_or < min_qubits_or) {
							swap(max_qubits_or, min_qubits_or);
							max_reg_or = qvar2;
						}

						QuantumVariable* ancilla_nbit_or = alloc_ancilla(qc, max_qubits_or, table);
						
						// OR common bits
						for(int qbit_pair = max_qubits_or - min_qubits_or; qbit_pair < max_qubits_or; qbit_pair++) {
							bitwise_or(qc, qvar1, qbit_pair, qvar2, qbit_pair, ancilla_nbit_or, qbit_pair);
						}

						// OR uncommon bits (case where one register is larger than the other, then OR with zero is simply copying the bit value
						for(int qbit_cutoff = 0; qbit_cutoff < max_qubits_or - min_qubits_or; qbit_cutoff++) {
							qc.cx(max_reg_or->get_qreg(), qbit_cutoff, ancilla_nbit_or->get_qreg(), qbit_cutoff); 
						}
						
						// NOT entire register
						qc.x(ancilla_nbit_or->get_qreg());

						QuantumVariable* ancilla_mcgate = alloc_ancilla(qc, ancilla_nbit_or->get_num_qubits()-1, table);
						multi_ctrl_gate(qc, &Circuit::cx, ancilla_nbit_or, result_reg, ancilla_mcgate);
						
						// Uncompute NOT
						qc.x(ancilla_nbit_or->get_qreg());

						// Uncompute OR operation on ancilla

						// OR common bits
						for(int qbit_pair = max_qubits_or - min_qubits_or; qbit_pair < max_qubits_or; qbit_pair++) {
							bitwise_or(qc, qvar1, qbit_pair, qvar2, qbit_pair, ancilla_nbit_or, qbit_pair);
						}

						// OR uncommon bits (case where one register is larger than the other, then OR with zero is simply copying the bit value
						for(int qbit_cutoff = 0; qbit_cutoff < max_qubits_or - min_qubits_or; qbit_cutoff++) {
							qc.cx(max_reg_or->get_qreg(), qbit_cutoff, ancilla_nbit_or->get_qreg(), qbit_cutoff); 
						}

						continue;

					}

					qc.ccx(qvar1->get_qreg(), 0, qvar2->get_qreg(), 0, result_reg->get_qreg(), 0);
					qc.cx(qvar1->get_qreg(), 0, result_reg->get_qreg(), 0);
					qc.cx(qvar2->get_qreg(), 0, result_reg->get_qreg(), 0);
				}

				else if(type == "^") {
					qc.cx(qvar1->get_qreg(), 0, result_reg->get_qreg(),0);
					qc.cx(qvar2->get_qreg(), 0, result_reg->get_qreg(),0);

				}


			}

			else {
				
				// get which operand is a number and store it
				string num = isNumeric(reg0) ? reg0 : reg1;
			
				// get which operand is an identifier and store it
				QuantumVariable* qvar = table->search_qtable(isIdentifier(reg0) ? reg0 : reg1);
				
				// compile addition
				if (type == "+") {
						add(qc, *qvar, stoll(num), *result_reg, instruction.is_inverted(), instruction.get_controls());
					
				}

				// compile subtraction
				else if (type == "-") {
						subtract(qc, *qvar, stoll(num), *result_reg, instruction.get_controls());

					
				}

				// compile multiplication
				else if (type == "*") {
					multiply_by_int(qc, *qvar, stoll(num), *result_reg, instruction.is_inverted());
				}


				else if (type == "%") {
					modulo_by_two_exp(qc, *qvar, stoll(num), *result_reg);
					
				}

				// compile addition-append
				else if (type == "+=") {
					add_append(qc, *qvar, stoll(num), instruction.is_inverted(),instruction.get_controls());
					
				}

				// compile subtraction-append
				else if (type == "-=") {
					subtract_append(qc, *qvar, stoll(num));
				}

				// compile multiplication-append
				else if (type == "*=") {
					
					multiply_append_by_int(qc, *qvar, stoll(num));
				}

				// compile relational (comparison) operators
				if (isCompOp(type)) {
					// create numeric register
					QuantumVariable* qvar2;
					// check if numeric register already exists
					// it doesn't, create a numeric register
					if (!table->is_searchable(get_num_to_words(num))) qvar2 = create_num_reg(qc, stoll(num), 0, *table);

					// else search it up
					else qvar2 = table->search_qtable(get_num_to_words(num));

					// get number of qubits for ancillary register. It is the size of the largest register
					int num_qubits_ancilla = qvar->get_num_qubits() > qvar2->get_num_qubits() ? qvar->get_num_qubits() : qvar2->get_num_qubits();
				
					
					QuantumVariable* ancilla = Garbage::get_garbage()->get_recycled(num_qubits_ancilla, table);
					if(ancilla == nullptr) {
						// create ancillary register
						table->ADD_ANCILLA_REGISTER(num_qubits_ancilla);

						// get ancillary register
						ancilla = table->search_qtable(table->GET_ANCILLA_REGISTER());				

					}
						qc.add_qregister(*ancilla);
					
					// compile equality operator
					if (type == "==") {
	
						circ_equal_to(qc, *qvar, *qvar2, *result_reg, *ancilla);
					 }

					else if(type == "!=") {
						circ_equal_to(qc, *qvar, *qvar2, *result_reg, *ancilla);
						qc.x(result_reg->get_qreg());
					}
					
					// compile greater-than operator
					else if (type == ">") {
						greater_than(qc, *qvar, *qvar2, *result_reg, *ancilla);
					}

					// compile greater-than-or-equal-to operator
					else if (type == ">=") {
						
						greater_or_equal(qc, *qvar, *qvar2, *result_reg, *ancilla);
					}

					// compile less-than operator
					else if (type == "<") {
						
						less_than(qc, *qvar, *qvar2, *result_reg, *ancilla);
					}

					// compile less-than-or-equal to
					else if (type == "<=") {
						
						less_or_equal(qc, *qvar, *qvar2, *result_reg, *ancilla);
					}

					// add ancillary register to recycling bin
					Garbage::get_garbage()->add_garbage(table->search_qtable(table->GET_ANCILLA_REGISTER()));

				}


			}
		}

		// For built-in-function or assembly operator
		else if(instruction.get_value() == "PREDEF_FUNC") {
						
			// get name of function
			string func_name = instruction.GET_PF().get_func_name();

			// get function parameter names
			vector<string> values = instruction.GET_PF().get_parameters();

			// compile Assembly Instruction
			if (isAssembly(func_name)) {
				create_assembly(qc, instruction);
			}
			
			// for built-in-functions
			else {
				// iterate over each function parameter
				for (int val = 0; val < values.size(); val++) {
					string parameter = values[val];

					// compile measurement
					if (func_name == "measure") {
						// create classical register
						ClassicalVariable* creg = new ClassicalVariable(parameter);

						// add classical register to symbol-table
						table->push_classical_var(creg);

						// add classical register to circuit
						// classical register takes the form of creg_<QuantumVariableName>
						qc.add_cregister("creg_" + parameter, table->search_qtable(parameter)->get_num_qubits());

						// measure
						qc.measure(parameter, "creg_" + parameter);

					}

					// compile "mark" function
					else if (func_name == "mark") {

						// add CMP register (1 qubit)
						table->ADD_CMP_REGISTER();

						// add CMP register to circuit
						qc.add_qregister(*table->search_qtable(table->GET_CMP_REGISTER()));

						// Phase-Kickback
						// Kick-back phase of pi (3.14159...)
						qc.x(table->GET_CMP_REGISTER());
						long double phase;
						if(values[val+1] == "pi") phase = pi;
						else phase = stod(values[val+1]);
						qc.cu(instruction.get_controls()[0].get_control(), 0, table->GET_CMP_REGISTER(), 0, phase);
						qc.x(table->GET_CMP_REGISTER());

						// we need not iterate over remaining values, therefore we must break the loop
						break;
					}

					// compile "Diffuse" function
					else if (func_name == "DIFFUSE") {
						// get input to apply amplitude amplification
						QuantumVariable* input = table->search_qtable(parameter);
						QuantumVariable* ancilla = nullptr;
						// add ancillary register to symbol-table
						if(input->get_num_qubits() > 3) {
							ancilla = Garbage::get_garbage()->get_recycled(input->get_num_qubits()-1, table);
							
							if(ancilla == nullptr) {
								table->ADD_ANCILLA_REGISTER(input->get_num_qubits() - 1);
								// store ancillary register
								ancilla = table->search_qtable(table->GET_ANCILLA_REGISTER());
								// add circuit
							}
							
							
							qc.add_qregister(*ancilla);

						}

						table->ADD_CMP_REGISTER();
						QuantumVariable* target = table->search_qtable(table->GET_CMP_REGISTER());
						qc.add_qregister(*target);
						qc.x(target->get_qreg());
						AmplitudeAmplify(qc, input, target, ancilla);

					}

				}
			}
		}
	}
}


#endif
