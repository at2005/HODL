
#ifndef RESOURCES_H
#define RESOURCES_H

#include "estimation_funcs.h"

// THIS FUNCTION COMPUTES SIZE OF EACH REGISTER REQUIRED AND CREATES THEM


unsigned long long eval_resources(SyntaxTree* tree, SymbolTable* table, QuantumVariable* dependency, Node* expr_node, bool is_right_child, map<string, string> parameter_map) {
	// Root node for expression parsing
	//cout << tree;
	Node* node = tree->getRoot();
	
	//cout << node->getTValue() << ":" << node->getTToken() << endl;
	// Static variable to hold name of the variable a function returns
	static string func_return_name = "";

	// Get function table for function definition lookup
	FunctionTable* func_table = FunctionTable::get_func_table();

	//  check if function call
	bool func_call = !parameter_map.empty();

	// total number of resources
	int total_resources = 0;
	
	// Check if node is not NULL
	if (node != (Node*)NULL) {	
		// Classical Interpretation on node
		
		interpret(tree, *table);

		//case where value is classical
		if (node->getTToken() == "NUMBER" || node->getTToken() == "FLOAT") {
			// check if this value is for assignment
			if (dependency != nullptr) {
				// get number of qubits of value
				return log2(stoi(node->getTValue()));
			}

			// if not assignment
			node->set_result_register(node->getTValue());
			

			// return resources
			return eval_classical_resources(node);
			
		}


		// case where value is an identifier
		else if (node->getTToken() == "IDENTIFIER") {
			if (dependency != nullptr) {
				table->search_qtable(node->getTValue())->set_dependency(dependency, expr_node);
			}

			if (!func_call) {
				node->set_result_register(node->getTValue());
				return table->search_qtable(node->getTValue())->get_num_qubits();
			}


			node->set_result_register(parameter_map.find(node->getTValue())->second);
			return table->search_qtable(parameter_map.find(node->getTValue())->second)->get_num_qubits();
		}


		// case where the node is a built-in-function or assembly instruction
		else if (isBuiltInFunction(node->getTValue()) || isAssembly(node->getTValue())) {
			// iterate over function parameters
			for (int i = 0; i < tree->get_function_parameters().size(); i++) {
				// evaluate resources of function parameters
				eval_resources(&tree->get_function_parameters()[i], table, dependency, expr_node, is_right_child, parameter_map);
			}
		}

		// case where the node is a user-defined-function
		else if (node->getTToken() == "FCALL" && !(isBuiltInFunction(node->getTValue()))) {
			//get function table
			Function* func = func_table->search_global_func_stack(node->getTValue());

			// create child trees
			vector<SyntaxTree>& child_trees = func->get_syntax_tree()->get_child_trees();

			// create new parameter map - initialized to input parameter map
			// This creates maps all input values to their position in the function definition
			map<string, string> new_param_map = parameter_map;

			// parameters of function call
			vector<SyntaxTree> params = tree->get_function_parameters();

			// create a new symbol table
			SymbolTable* func_scope_table = new SymbolTable();

			// add add parent symbol table to new table
			func_scope_table->add_existing_table(table);

			// set the symbol table of the syntax tree
			func->get_syntax_tree()->set_table(func_scope_table);

			// add values to parameter map
			for (int parameter = 0; parameter < params.size(); parameter++) {
				// Evaluate resources on parameters
				eval_resources(&params[parameter], func_scope_table, dependency, expr_node, is_right_child, new_param_map);
				//cout << "hello world\n"
				// Insert values into the parameter map
				new_param_map.insert({ func->get_syntax_tree()->get_function_parameters()[parameter].getRoot()->getTValue(),tree->get_function_parameters()[parameter].getRoot()->getTValue() });
			}

			//estimate resources of child trees and set return register
			// iterate over all child trees
			for (int i = 0; i < child_trees.size(); i++) {
				// estimate resources for child trees
				eval_resources(&child_trees[i], func_scope_table, dependency, expr_node, is_right_child, new_param_map);

				// check if return statement
				if (child_trees[i].getRoot()->getTValue() == "return") {
					// set name of variable function returns
					func_return_name = child_trees[i].get_function_parameters()[0].getRoot()->getTValue();

					// push quantum variable to parent table of current table
					table->push_quantum_var(func_scope_table->search_qtable(func_return_name));

					// set result register 
					node->set_result_register(func_return_name);

					// return size of variable that function returns
					return table->search_qtable(func_return_name)->get_num_qubits();
				}
			}

			// else return zero
			return 0;
		}

		// evaluate resources for FOR LOOP	
		else if (node->getTValue() == "for") {
			// iterate over body of LOOP
			for (auto& child : tree->get_child_trees()) {
				// evaluate resources
				SyntaxTree child_tree(child);
				eval_resources(&child_tree, table, dependency, expr_node, is_right_child, parameter_map);
			}
		}


		//case for operators
		else if (node->getTToken() == "OPERATOR") {
			// get left value
			string left = node->getLeftChild()->getTValue();

			// get right value
			string right = node->getRightChild()->getTValue();


			// if function call, map values to their function parameters
			if (func_call) {
				// map left child
				if (parameter_map.find(left) != parameter_map.end()) left = parameter_map.find(left)->second;

				// map right child
				if (parameter_map.find(right) != parameter_map.end()) right = parameter_map.find(right)->second;
			}

			// if right child, create ancillary register to hold result
			if (is_right_child) {
				// check if node has no result register
				if (node->get_result_register() == "") {
					// create AX register
					table->ADD_AX_REGISTER();

					// set result register to AX register
					node->set_result_register(table->GET_AX_REGISTER());
				}
			}

			// if no resulting output, create result register
			else if (!is_right_child && dependency == nullptr) {
				// check if not append operator, e.g. +=, *=, -=
				if (!isAppendOperator(node->getTValue())) {

					// check if node has not already been assigned a result register
					if (node->get_result_register() == "") {

						// add AX register
						table->ADD_AX_REGISTER();

						// set node's result register to AX register
						node->set_result_register(table->GET_AX_REGISTER());
					}
				}
			}

			else {

				if (node->getParent()) {
					if (node->getParent()->getTValue() == "=") {
						node->set_result_register(dependency->get_name());
					}

					else {
						node->set_result_register(node->getParent()->get_result_register());
					}
				}

				else if (dependency != nullptr) {
					node->set_result_register(dependency->get_name());
				}

			}

			// for append operators, e.g. +=, -=, *=
			if (isAppendOperator(node->getTValue())) {
				// search table for left register
				QuantumVariable* qvar = table->search_qtable(left);


				SyntaxTree tree_right(node->getRightChild());
				int num_resources = eval_resources(&tree_right, table, qvar, node, false, parameter_map);
				node->set_result_register(qvar->get_qreg());
				if (node->getTValue() == "+=") {
					qvar->set_num_qubits(estimate_addition_append(qvar->get_num_qubits(), num_resources));
				}

				else if (node->getTValue() == "*=") {
					qvar->set_num_qubits(estimate_multiplication(qvar->get_num_qubits(), num_resources));
				}
			}

			//if both values are identifiers
			// idk what this is meant to do i forgot why i even have this
			else  if (isIdentifier(left) && isIdentifier(right)) {
				if (parameter_map.find(left) != parameter_map.end()) left = parameter_map.find(left)->second;
				if (parameter_map.find(right) != parameter_map.end()) right = parameter_map.find(right)->second;
				node->getLeftChild()->set_result_register(left);
				node->getRightChild()->set_result_register(right);

				if (dependency != nullptr) {
					table->search_qtable(left)->set_dependency(dependency, expr_node);
					table->search_qtable(right)->set_dependency(dependency, expr_node);
				}

				unsigned long long id_resources = eval_id_resources(node, left, right, table);
				if (is_right_child) {
					QuantumVariable* qvar = table->search_qtable(node->get_result_register());
					qvar->set_num_qubits(id_resources);
				}

				QuantumVariable* qleft = table->search_qtable(left);
				QuantumVariable* qright = table->search_qtable(right);

				QuantumVariable* result = table->search_qtable(dependency->get_qreg());
				result->add_reference(qleft);
				result->add_reference(qright);
				return id_resources;

			}


			//check if both values are numbers
			else if (isNumeric(left) && isNumeric(right)) {
				if (node->getTValue() == ":") {
					return estimate_range(stoll(right));
				}
			}



			//check if one value is a number and the other is an identifier
			else if ((isIdentifier(left) || isIdentifier(right)) && (isNumeric(left) || isNumeric(right))) {
				//cout << node->getTValue();
				string id = getIdentifier(left, right);
				if (parameter_map.find(id) != parameter_map.end()) id = parameter_map.find(id)->second;

				node->getLeftChild()->set_result_register(node->getLeftChild()->getTValue());
				node->getRightChild()->set_result_register(node->getRightChild()->getTValue());



				if (dependency != nullptr) {
					table->search_qtable(id)->set_dependency(dependency, expr_node);

				}


				unsigned long long id_num_resources = eval_id_num_resources(node, left, right, table);

				if (is_right_child || (!is_right_child && dependency == nullptr)) {
					QuantumVariable* qvar = table->search_qtable(node->get_result_register());
					qvar->set_num_qubits(id_num_resources);
				}

				QuantumVariable* qvar = table->search_qtable(id);
				table->search_qtable(node->get_result_register())->add_reference(qvar);
				return id_num_resources;
			}



			else {
				//goes into recursion for child nodes that are operators
				if (node->getLeftChild() && node->getRightChild()) {
					eval_operator_child(node, table, total_resources, expr_node, dependency, parameter_map);
				}

				else {
					return 0;
				}
			}

		}



		// for conditionals
		else if (node->getTValue() == "if" || node->getTValue() == "elsif") {

			if(node->getLeftChild()->is_classical()) {
				if(tree->getcf()) {
					for(auto& child: tree->get_child_trees()) {
						eval_resources(&child, table, nullptr, nullptr, false, parameter_map);

					}
				}

				return 0;

			}

			SyntaxTree tree_left(node->getLeftChild());

			// evaluate resources for condition
			eval_resources(&tree_left, table, nullptr, nullptr, false, parameter_map);
			// add CMP register
			table->ADD_CMP_REGISTER();

			// set result register
			node->getLeftChild()->set_result_register(table->GET_CMP_REGISTER());
			for(auto& child: tree->get_child_trees()) {
				eval_resources(&child, table, nullptr, nullptr, false, parameter_map);

			} 
		}


		// for relational operations, (e.g. > , < , == , !=) and logical operators (e.g. & , |)
		else if (node->getTToken() == "COMPARISON_OPERATOR" || node->getTToken() == "BOOL_EXPR") {
			SyntaxTree tree_left(node->getLeftChild());
			SyntaxTree tree_right(node->getRightChild());
			// evaluate resources for left and right child
			eval_resources(&tree_left, table, nullptr, nullptr, false, parameter_map);
			eval_resources(&tree_right, table, nullptr, nullptr, false, parameter_map);

			// create a CMP register to store boolean result
			table->ADD_CMP_REGISTER();

			// set result register
			node->set_result_register(table->GET_CMP_REGISTER());
		}



		//evaluate resources for assignment
		else if (node->getTToken() == "ASSIGNMENT") {

			SyntaxTree* tree_right = new SyntaxTree(node->getRightChild());
			//for quantum assignment
			string left = node->getLeftChild()->getTValue();
			// check if quantum variable
			if (node->getLeftChild()->getPurpose() == "super") {
				// create new quantum variable
				QuantumVariable* qvar = new QuantumVariable(left);
				// add quantum variable to symbol table
				table->push_quantum_var(qvar);

				// evaluate resources
				int resources = eval_resources(tree_right, table, qvar, node, false, parameter_map);
												//if(node->getTValue() == "=") cout << left;
				// set number of qubits register requires
				qvar->set_num_qubits(resources);

				// for assigning results of function calls
				if (func_return_name != "") {
					// share a pointer (for the number of resources) with the returned value
					qvar->set_num_pointer(table->search_qtable(func_return_name)->get_num_pointer());

					// set quantum register to function return value register
					qvar->set_qreg(func_return_name);
				}
				

				// reset function return value
				func_return_name = "";

				// set result register of node
				node->set_result_register(qvar->get_qreg());

				// return resources
				return resources;

			}
		}

		// return total number of qubits

		return total_resources;
	}

	// return zero should nothing be returned

	return 0;

}

#endif
