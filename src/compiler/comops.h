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
					if (qvar1->get_num_qubits() > 1 || qvar2->get_num_qubits() > 1) {
						// n qubit OR gate, loop through all qubit pairs, eval or on ancilla, then X all of them, multi-controlled gate
						QuantumVariable* max_reg_and = qvar1;
						QuantumVariable* min_reg_and = qvar2;
						// get min and max qubits
						unsigned int max_qubits_and = qvar1->get_num_qubits();
						unsigned int min_qubits_and = qvar2->get_num_qubits();
						if(max_qubits_and < min_qubits_and) {
							swap(max_qubits_and, min_qubits_and);
							swap(max_reg_and, min_reg_and);
						}

						// check if single qubit OR required or register-wide or
						QuantumVariable* ancilla_nbit_and = result_reg;
						
						if(result_reg->get_num_qubits() == 1) {
							ancilla_nbit_and = alloc_ancilla(qc, max_qubits_and, table);
						
						}	
				
						unsigned int diff = max_qubits_and - min_qubits_and;
						// and all common qubits
						for(int qbit_pair = diff; qbit_pair < max_qubits_and; qbit_pair++) {
							qc.ccx(max_reg_and->get_qreg(), qbit_pair, min_reg_and->get_qreg(), qbit_pair-diff, ancilla_nbit_and->get_qreg(), qbit_pair-diff);
						}
						
						
						// continue if not in if statement						
						if(result_reg->get_num_qubits() > 1) continue;
					

						// ELSE multi-controlled NOT onto single qubit

						qc.x(ancilla_nbit_and->get_qreg());
						qc.x(result_reg->get_qreg());
						

						QuantumVariable* ancilla_mcgate = alloc_ancilla(qc, ancilla_nbit_and->get_num_qubits()-1, table);
						multi_ctrl_gate(qc, &Circuit::cx, ancilla_nbit_and, result_reg, ancilla_mcgate);
						

						// uncomputation
						qc.x(ancilla_nbit_and->get_qreg());
							
						for(int qbit_pair = max_qubits_and - min_qubits_and; qbit_pair < max_qubits_and; qbit_pair++) {
							qc.ccx(qvar1->get_qreg(), qbit_pair, qvar2->get_qreg(), qbit_pair, ancilla_nbit_and->get_qreg(), qbit_pair);
						}
						
						Garbage::get_garbage()->add_garbage(ancilla_mcgate);		
						Garbage::get_garbage()->add_garbage(ancilla_nbit_and);						
						continue;
					}


					
					qc.ccx(qvar1->get_qreg(), 0, qvar2->get_qreg(), 0, result_reg->get_qreg(), 0);
				}	

				// compile OR operator
				else if (type == "|") {
					if(qvar1->get_num_qubits() > 1 || qvar2->get_num_qubits() > 1) {
						// n qubit OR gate, loop through all qubit pairs, eval or on ancilla, then X all of them, multi-controlled gate
						
						QuantumVariable* max_reg_or = qvar1;
						QuantumVariable* min_reg_or = qvar2;
						// get min and max qubits
						unsigned int max_qubits_or = qvar1->get_num_qubits();
						unsigned int min_qubits_or = qvar2->get_num_qubits();
						if(max_qubits_or < min_qubits_or) {
							swap(max_qubits_or, min_qubits_or);
							swap(max_reg_or, min_reg_or);
						}

						// check if single qubit OR required or register-wide or
						QuantumVariable* ancilla_nbit_or = result_reg;
						
						if(result_reg->get_num_qubits() == 1) {
							ancilla_nbit_or = alloc_ancilla(qc, max_qubits_or, table);
						
						}	
	
						unsigned int diff = max_qubits_or - min_qubits_or;	
						// OR common bits
						for(int qbit_pair = max_qubits_or - 1; qbit_pair >= diff; qbit_pair--) {
							bitwise_or(qc, max_reg_or, qbit_pair, min_reg_or, qbit_pair-diff, ancilla_nbit_or, qbit_pair);
						}

						// OR uncommon bits (case where one register is larger than the other, then OR with zero is simply copying the bit value
						for(int qbit_cutoff = 0; qbit_cutoff < diff; qbit_cutoff++) {
							qc.cx(max_reg_or->get_qreg(), qbit_cutoff, ancilla_nbit_or->get_qreg(), qbit_cutoff); 
						}
						

						// continue if not in if statement						
						if(result_reg->get_num_qubits() > 1) continue;
						

						// Following block only for if statement bool

						// NOT entire register
						qc.x(ancilla_nbit_or->get_qreg());

						// NOT result register
						qc.x(result_reg->get_qreg());

						QuantumVariable* ancilla_mcgate = alloc_ancilla(qc, ancilla_nbit_or->get_num_qubits()-1, table);
						multi_ctrl_gate(qc, &Circuit::cx, ancilla_nbit_or, result_reg, ancilla_mcgate);
						
						// Uncompute NOT
						qc.x(ancilla_nbit_or->get_qreg());

						// Uncompute OR operation on ancilla - START	
						// OR common bits
						for(int qbit_pair = max_qubits_or - 1; qbit_pair >= diff; qbit_pair--) {
							bitwise_or(qc, max_reg_or, qbit_pair, min_reg_or, qbit_pair-diff, ancilla_nbit_or, qbit_pair);
						}

						// OR uncommon bits (case where one register is larger than the other, then OR with zero is simply copying the bit value
						for(int qbit_cutoff = 0; qbit_cutoff < diff; qbit_cutoff++) {
							qc.cx(max_reg_or->get_qreg(), qbit_cutoff, ancilla_nbit_or->get_qreg(), qbit_cutoff); 
						}
						// END UNCOMPUTATION

//						Garbage::get_garbage()->add_garbage(ancilla_mcgate);		
//						Garbage::get_garbage()->add_garbage(ancilla_nbit_or);						
						
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

				// compile range operator
				else if (type == ":") {
					// get number or right operand
					long long number = stoll(reg1);

					// initialize number of qubits to zero
					int num_qubits = 0;

					// check if range is a power-of-two
					// in other words, check if log2(number) is an element of the set consisting natural numbers
					if (reg0 == "0" && ceil(log2(number)) == floor(log2(number))) {
						// set number of qubits
						// the reason for this is to only create a superposition of the range desired.
						// e.g., a register of 5 qubits but we want a superposition of 0:4
						// Therefore, we have to only put the last 2 qubits into superposition
						int num_qubits = floor(log2(number));

						// Apply HADAMARD Gate to each qubit we want to be in superpositon
						for (int i = (result_reg->get_num_qubits() - num_qubits); i < result_reg->get_num_qubits(); i++) {
							qc.h(result_reg->get_qreg(), i);
						}

					}

					// this should be implemented in later versions
					// to allow programmer to specify custom range, e.g. (1:100)
					else {
						int num_qubits = floor(log2(number)) + 1;
						//apply operation to procure such a state
					}
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
