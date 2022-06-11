
#ifndef GEN_INSTRUCTIONS_H
#define GEN_INSTRUCTIONS_H


#include "conditionals.h"
#include "instructions.h"
#include "../lexer/tokens.h"
#include "../tables/ftable.h"
#include <sstream>


struct oracle_data {
	// holds pointer to function definition
	Function* func_def;

	// holds pointer to parameter map
	map<string, string>* param_map;

	// holds symbol table for function
	SymbolTable* st;

};

static struct oracle_data* ptr_to_orac = nullptr;

//create instructions
string create_instructions(vector<INSTRUCTION>& instructions, SyntaxTree* tree, map<string, string> param_map = {}, SymbolTable* table = nullptr, vector<Conditional> controls = {}, bool invert = false) {
	//cout << "hello";
	Node* root = nullptr;
	root = tree->getRoot();
	//NULL if both values non-existent
	Node* left = nullptr;
	left = root->getLeftChild();
	Node* right = nullptr;
	right = root->getRightChild();
	
	//cout << root->getTValue();
	// base case for identifiers or numbers
	if (root->getTToken() == "IDENTIFIER" || root->getTToken() == "NUMBER") {
		// get value of node
		string value = root->getTValue();

		// check if value is a function parameter
		if (param_map.find(value) != param_map.end()) {
			value = param_map.find(value)->second;
		}

		// look up value in a symbol table and return the qreg associated with it
		if (table->is_searchable(value)) {
			value =  table->get_qtable().find(value)->second->get_qreg();
		}
	
		return value;
	
	}

	// case for loops
	else if (root->getTToken() == "LOOP") {
		// iterate over each child tree in loop body
		for (SyntaxTree& child_tree : tree->get_child_trees()) {
			// create instructions for child tree
			create_instructions(instructions, &child_tree, param_map, table);
		}
	}
	
	// case for keywords/built-in-functions/assembly instructions
	else if (root->getTToken() == "KEYWORD" || isBuiltInFunction(root->getTValue()) || isAssembly(root->getTValue())) {
		// parameters
		vector<string> params = {};

		// iterate over function parameters
		for (SyntaxTree& parameter : tree->get_function_parameters()) {
			// create instructions for each parameter and store the output register
			string instruction_param = create_instructions(instructions, &parameter, param_map, table, controls, invert);
			// store parameter register in a vector
			params.push_back(instruction_param);
		}

		// case for the filter function which performs the Diffusion operator
		if (root->getTValue() == "diffuse") {

			// apply diffusion operation
			instructions.push_back(INSTRUCTION("DIFFUSE", tree->get_function_parameters()[0].getRoot()->getTValue()));
	
			
			// delete root node
			delete root;
			
			// return empty string
			return "";
		}
		
		// if conditional, then apply instruction conditional
		if (!controls.empty()) instructions.push_back(INSTRUCTION(root->getTValue(), params, controls));
		
		// else if not conditional, then push back instruction with no conditional
		else instructions.push_back(INSTRUCTION(root->getTValue(), params));
		
		// to be uncomputed values for return expression
		vector<QuantumVariable*> to_be_uncomputed = {};

		// case for keyword return
		if (root->getTValue() == "return") {	
			// iterate over each variable in parameters
			for (auto& return_var : params) {
				// set them equal flag equal to true so as to not uncompute them
				table->search_qtable(return_var)->flag = true;
			}
			
			// iterate over each quantum variable in qtable
			for (auto& qvar : table->get_qtable()) {

				// conditional that variable carries references and gives references
				bool cond1 = qvar.second->get_carry_ref().size() && qvar.second->get_given_ref().size();

				// condition that variable neither carries nor gives references 
				bool cond2 = (qvar.second->get_carry_ref().size() == 0) && qvar.second->get_given_ref().size() == 0;

				// condition that the variable has not set its flag
				bool cond3 = !qvar.second->flag;

				// either condition 1 or condition 2, but condition 3 has to be set
				if ((cond1 || cond2) && cond3) {
					// add this to the list of variables to uncompute
					to_be_uncomputed.push_back(qvar.second);
				}
			}

			// current iterator in instruction tape, set to instruction prior to return
			vector<INSTRUCTION>::iterator current = instructions.end()-2;

			// instructions to be uncomputed
			vector<INSTRUCTION> instructions_uncompute = {};

			// roll-back until instruction hits beginning of scope
			while (current->get_value() != "BEGIN") {
				// if instruction is an operation - this should be changed to support functions in the future.
				if (current->get_value() == "OPERATION") {
					// iterate over each quantum variable to be uncomputed
					for (auto& qvar : to_be_uncomputed) {
						// if the result register for instruction is the same as the register for the quantum variable
						if (current->GET_OP().get_result() == qvar->get_qreg()) {
							// push back inverted instruction
							instructions_uncompute.push_back(current->inverse());
						}
					}
				}

				// decrement iterator
				current--;

			}

			// append instructions to be uncomputed 
			for (auto& i : instructions_uncompute) {
				instructions.push_back(i);
			}
		}
		
		// delete root
		delete root;
	} 


	// case for instruction for operator 
	else if (root->getTToken() == "OPERATOR" || root->getTToken() == "COMPARISON_OPERATOR" || root->getTToken() == "BOOL_EXPR") {
		
		// if left and right exists 
		SyntaxTree left_tree = (SyntaxTree)left;
		SyntaxTree right_tree = (SyntaxTree)right;
		if (left && right) {
			// get left register
			string left_reg = create_instructions(instructions, &left_tree, param_map, table,controls);

			// get right register
			string right_reg = create_instructions(instructions, &right_tree, param_map, table, controls);

			// if conditional, push conditional instruction  
			if(!controls.empty()) instructions.push_back(INSTRUCTION(left_reg, right_reg, root->getTValue(), root->get_result_register(), controls));

			// else push no-conditonal instruction
			else instructions.push_back(INSTRUCTION(left_reg, right_reg, root->getTValue(), root->get_result_register()));
		
		}

		// result register
		string result = root->get_result_register();
	
		// delete root
		delete root;

		// return result register
		return result;
	}


	// generate instruction for assignment

	else if (root->getTToken() == "ASSIGNMENT") {
		// for quantum assignmnet
		if (root->getLeftChild()->getPurpose() == "super") {
			// if the value on the right is a number
			if (isNumeric(right->getTValue())) {
				// push back instruction
				if (!controls.empty()) instructions.push_back(INSTRUCTION("0", right->getTValue(), ":", root->get_result_register(), controls));
				else instructions.push_back(INSTRUCTION("0", right->getTValue(), ":", root->get_result_register()));

				// delete root
				delete root;

				// return null string
				return "";
			}

			// if value on the right is an expression
			else {
				SyntaxTree left_tree = (SyntaxTree)left;
				SyntaxTree right_tree = (SyntaxTree)right;
		
				// for conditionals, create instructions
				if (!controls.empty()) create_instructions(instructions, &right_tree, param_map, table, controls);

				else {
					// for non-conditionals, create instructions
					create_instructions(instructions, &right_tree, param_map, table);
				}
				
				// return left value (variable name)
				return left->getTValue();
			}
		}

		// delete root
		delete root;
	}

	// case for if statement
	else if (tree->get_expr_type() == "IF_STATEMENT" || tree->get_expr_type() == "ELSE_IF_STATEMENT") {
		// case for classical conditionals which are to be executed
		if(tree->getRoot()->getLeftChild()->is_classical()) {
			if(tree->getcf()) {
				for(auto& child: tree->get_child_trees()) {
					create_instructions(instructions, &child, param_map, table);

				}

			}
			
			return "";

		}


		// get size of instructions at first stage
		unsigned int size_of_instructions = instructions.size();
		SyntaxTree lltree = (SyntaxTree)tree->getRoot()->getLeftChild()->getLeftChild();
		SyntaxTree lrtree = (SyntaxTree)tree->getRoot()->getLeftChild()->getRightChild();
		// create left register of the conditional
		string left_reg = create_instructions(instructions, &lltree, param_map, table);

		// create right register of the conditional
		string right_reg = create_instructions(instructions, &lrtree, param_map, table);

		// push back conditional instruction
		instructions.push_back(INSTRUCTION(left_reg, right_reg, root->getLeftChild()->getTValue(), root->getLeftChild()->get_result_register()));

		// get size of instructions at second stage
		unsigned int size_of_instructions2 = instructions.size();
	
		// create a control register
		Conditional control(root->getLeftChild()->get_result_register());

		// add control to controls
		controls.push_back(control);

		// iterate over each child tree in conditional body
		for (int i = 0; i < tree->get_child_trees().size(); i++) {
			SyntaxTree child_tree_instance = (SyntaxTree)tree->get_child_trees()[i];
			// create instructions for each child tree in conditonal body
			create_instructions(instructions, &child_tree_instance, param_map, table, controls);
		}

		// define current iterator, as in, the position the instruction tape is at minus the amount of space the conditional
		// body takes up
		vector<INSTRUCTION>::iterator current_iter = instructions.begin() + size_of_instructions2 - 1;

		// define a rollback_to iterator, as in, the position we wish to roll-back to
		vector<INSTRUCTION>::iterator rollback_to = instructions.begin() + size_of_instructions - 1;

		// create a temporary instruction tape
		vector<INSTRUCTION> temp_tape= {};

		// iteratre backwards from current iterator to the iterator we wish to roll-back to
		for (;current_iter > rollback_to; current_iter--) {
				// get the inverse of each instruction
				INSTRUCTION instruction = current_iter->inverse();
				// punch the inverted instruction into our temporary instruction tape
				temp_tape.push_back(instruction);
			
		}

		// iterate over contents of temporary tape
		for (vector<INSTRUCTION>::iterator iter = temp_tape.begin(); iter < temp_tape.end(); iter++) {
			// punch inverted instructions from temporary tape to instruction tape
			instructions.push_back(*iter);
		}
		
		// delete root
		delete root;

	}

	// for else statement
	else if (tree->get_expr_type() == "ELSE_STATEMENT") {
		// get CMP register from previous conditional
		string cmp_result_reg = instructions[instructions.size() - 1].GET_PF().get_parameters()[0];

		// if the CMP register is not NULL
		if (cmp_result_reg != "") {

			// punch X instruction on CMP
			instructions.push_back(INSTRUCTION("X", cmp_result_reg));
			
			// create conditional vector
			vector<Conditional> controls2 = { Conditional(cmp_result_reg) };

			// create instructions for conditional body
			for (int i = 0; i < tree->get_child_trees().size(); i++) {
				SyntaxTree child_tree_instance = (SyntaxTree)tree->get_child_trees()[i];
				create_instructions(instructions, &child_tree_instance, param_map, table, controls2);
			}
			
			// punch X instruction on CMP
			instructions.push_back(INSTRUCTION("X", cmp_result_reg));

		}
	}

	// for custom function calls
	else if (root->getTToken() == "FCALL" && !(isBuiltInFunction(root->getTValue()) || isAssembly(root->getTValue()))) {
		// get function table
		FunctionTable* func_table = FunctionTable::get_func_table();

		// get function definition pointer
		Function* func = func_table->search_global_func_stack(root->getTValue());

		// get function body
		SyntaxTree* func_tree = func->get_syntax_tree();

		// initialize parameter map
		map<string, string>* parameter_map = new map<string,string>;
		// throw error if number of parameters required for function call are incorrect
		if (func_tree->get_function_parameters().size() != tree->get_function_parameters().size()) {
			// change color to red
			for(int i = 0; i < tree->getRoot()->get_func_params().size(); i++) {
				

			}
			cout << "FUNCTION " << func_tree->getRoot()->getTValue() << " DOES NOT TAKE " << tree->getRoot()->get_func_params().size() << " PARAMETERS";
			
			exit(1);
		}

		// iterate over each parameter in function parameters
		for (int parameter = 0; parameter < func_tree->get_function_parameters().size(); parameter++) {
			SyntaxTree parameter_tree = (SyntaxTree)tree->get_function_parameters()[parameter];
			//  generate parameter map
			parameter_map->insert({ func_tree->get_function_parameters()[parameter].getRoot()->getTValue(), create_instructions(instructions,&parameter_tree, param_map, table, controls, invert)});
		}

		// punch BEGIN instruction
		instructions.push_back(INSTRUCTION(func_tree->get_table(), "BEGIN"));

	
		// iterate over each child tree in function tree
		for (int child_tree = 0; child_tree < func_tree->get_child_trees().size(); child_tree++ ) {
			SyntaxTree& func_child_tree = func_tree->get_child_trees()[child_tree];
			// create each instruction
			create_instructions(instructions, &func_child_tree, *parameter_map, func_tree->get_table() , controls);
		}

		// punch END instruction
		instructions.push_back(INSTRUCTION(func_tree->get_table(), "END"));
		
		// delete parameter map
		delete parameter_map;
		
		
	
		// delete root	
		delete root;
	}

	// return default null string
	return "";
}

#endif
