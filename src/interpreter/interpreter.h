#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../parser/ast.h"
#include "../lexer/tokens.h"
#include <string>
#include "../tables/ftable.h"
#include "../tables/symtab.h"




// classical interpreter
long interpret(SyntaxTree* tree, SymbolTable& table, bool in_loop=false, map<string,string> param_map={}) {
	
	FunctionTable* func_table = FunctionTable::get_func_table();
	// get root of expression tree
	Node* node = tree->getRoot();
	// if root exists
	if (node != (Node*)(0xCCCCCCCC)) {
		// case for root being an assigmnent
		if (node->getTToken() == "ASSIGNMENT") {
			SyntaxTree tree_left(node->getLeftChild());
			SyntaxTree tree_right(node->getRightChild());
			// get value of right hand side
			
			long value = interpret(&tree_right, table, in_loop,param_map);
			// if the assignment type is classical, e.g. int
			if (node->getLeftChild()->getPurpose() == "int") {
				// create classical variable
				ClassicalVariable* variable = new ClassicalVariable(node->getLeftChild()->getTValue());
				// set value of variable to interpreted value of right-hand-side
				variable->set_value(value);
				// add classical variable in table
				table.push_classical_var(variable);
			}
	
			


		}

			

		// case for operation
		else if (node->getTToken() == "OPERATOR" || node->getTToken() == "COMPARISON_OPERATOR") {
			SyntaxTree tree_left(node->getLeftChild());
			SyntaxTree tree_right(node->getRightChild());
			// initialize result to zero
			long result = 0;

			// left is the left-hand-child
			string left = node->getLeftChild()->getTValue();

			// right is the right-hand-child
			string right = node->getRightChild()->getTValue();
			// if right node is an identifier
			if (node->getRightChild()->getTToken() == "IDENTIFIER") {
				// check if right node is a classical variable
				if (table.cvar_exists(right) || param_map.find(right) != param_map.end()) {
					// set flag to classical if so
					node->getRightChild()->set_classical();
				}



			}

			// if left node is an identifier
			if (node->getLeftChild()->getTToken() == "IDENTIFIER") {
				// check if left node is a classical node
				if (table.cvar_exists(left) || (param_map.find(left) != param_map.end())) {
					// set flag to classical if so
					node->getLeftChild()->set_classical();
				}

			}

			//goes into recursion
			if (node->getRightChild()->is_classical() && node->getLeftChild()->is_classical()) {
				// case for addition
				long lside =  interpret(&tree_left, table,in_loop, param_map);
				long rside = interpret(&tree_right, table, in_loop, param_map);
				if (node->getTValue() == "+") {
					// set result to sum of left and right side
					result = lside + rside;
				}

				
				// case for subtraction
				else if (node->getTValue() == "-") {
					// set result to difference of left and right side
					result = lside - rside;
				}

				// case for multiplication
				else if (node->getTValue() == "*") {
					// set result to product of left and right side
					result = lside * rside;
					
				}

				// case for division
				else if (node->getTValue() == "/") {
					// set result to ratio of left and right side
					result = lside / rside;

				}

				// case for addition-append
				else if (node->getTValue() == "+=") {
					// if variable on the left is a classical variable
					if (table.cvar_exists(left)) {
						// get classical variable
						ClassicalVariable* cvar = table.search_ctable(left);
						// increment its value by right-hand-side
						cvar->set_value(cvar->get_value() + rside);
					}
				}

				// case for subtraction-append
				else if (node->getTValue() == "-=") {
					if (table.cvar_exists(left)) {
						ClassicalVariable* cvar = table.search_ctable(left);
						cvar->set_value(cvar->get_value() - rside);
					}
				}

				else if (node->getTValue() == "*=") {
					if (table.cvar_exists(left)) {
						ClassicalVariable* cvar = table.search_ctable(left);
						cvar->set_value(cvar->get_value() * rside);
					}
				}

				else if (node->getTValue() == "/=") {
					if (table.cvar_exists(left)) {
						ClassicalVariable* cvar = table.search_ctable(left);
						cvar->set_value(cvar->get_value() / rside);
					}
				}


				else if (node->getTValue() == ">") {
					result = lside > rside;
				}

				else if (node->getTValue() == ">=") {
					result = lside >= rside;
				}

				else if (node->getTValue() == "<") {
					result = lside < rside;
				}

				else if (node->getTValue() == "<=") {
					result = lside <= rside;

				}

				else if (node->getTValue() == "==") {
					result = lside == rside;

				}

				else if (node->getTValue() == "!=") {
					result = lside != rside;

				}

				if (!in_loop) {
					////set operation node to result
					node->setTreeTV(Pair(to_string(result), "NUMBER"));
					node->deleteLeftChild();
					node->deleteRightChild();
					node->set_classical();
				}

				return result;
			}

			else if (node->getRightChild()->is_classical()) {
				result = interpret(&tree_right, table, in_loop, param_map);
				if (!in_loop) {
					*node->getRightChild() = Node(Pair(to_string(result), "NUMBER"));
				}
				return result;
			}

			else if (node->getLeftChild()->is_classical()) {
				result = interpret(&tree_left, table, in_loop, param_map);
				if (!in_loop) {
					*node->getLeftChild() = Node(Pair(to_string(result), "NUMBER"));
				}
				return result;
			}


		}


		else if (node->getTToken() == "NUMBER") {
			return stoi(node->getTValue());
		
		}

		else if (node->getTToken() == "IDENTIFIER") {
			if (table.cvar_exists(node->getTValue())) {
				long val = table.search_ctable(node->getTValue())->get_value();
				if (!in_loop) {
					node->setTreeTV(Pair(to_string(val), "NUMBER"));
				}
			
				return val;
			}

			
			if(param_map.size()) {
				if(param_map.find(node->getTValue()) != param_map.end()) {
					string val = param_map[node->getTValue()];
					node->setTreeTV(Pair(val, "NUMBER"));

					return stol(val);

				}

			}
		}



		else if(node->getTToken() == "FCALL" && !isBuiltInFunction(node->getTValue())) {
			Function* func = func_table->search_global_func_stack(node->getTValue());
			// create child trees
			vector<SyntaxTree>& child_trees = func->get_syntax_tree()->get_child_trees();

			// create new parameter map - initialized to input parameter map
			// This creates maps all input values to their position in the function definition
			map<string, string> new_param_map = param_map;

			// parameters of function call
			vector<SyntaxTree> params = tree->get_function_parameters();
			// add values to parameter map
			for (int parameter = 0; parameter < params.size(); parameter++) {
				// Insert values into the parameter map
				new_param_map.insert({ func->get_syntax_tree()->get_function_parameters()[parameter].getRoot()->getTValue(),tree->get_function_parameters()[parameter].getRoot()->getTValue() });
			}

			for(int child = 0; child < child_trees.size(); child++) {
				interpret(&(child_trees[child]), table, in_loop, new_param_map);


			}
			
				

		}
		

		
		else if(node->getTValue() == "if") {
			SyntaxTree* st = new SyntaxTree(node->getLeftChild());
			if(st->getRoot()->is_classical()) {
				if(interpret(st, table, false, param_map)) {
					tree->setcf();

				}
			}

		}

		else if (node->getTValue() == "for") {
			interpret(&tree->get_function_parameters()[0], table,true);
			vector<SyntaxTree> child_trees = {};
			while (interpret(&tree->get_function_parameters()[1], table, true, param_map)) {
				for (int i = 0; i < tree->get_child_trees().size(); i++) {
					interpret(&tree->get_child_trees()[i], table,true, param_map);
					child_trees.push_back(tree->get_child_trees()[i]);

				}
				
				interpret(&tree->get_function_parameters()[2], table, true, param_map);
				
			}
			
			tree->set_child_trees(child_trees);

		}

		
		else if(node->getTValue() == "while") {
			vector<SyntaxTree> child_trees = {};
			SyntaxTree* cond = new SyntaxTree(node->getLeftChild());
			
			while(interpret(cond, table, true, param_map)) {
				
				for(int i = 0; i < tree->get_child_trees().size(); i++) {					
					interpret(&tree->get_child_trees()[i], table,true, param_map);
					child_trees.push_back(tree->get_child_trees()[i]);

				}		
			
			}	
			
			tree->set_child_trees(child_trees);
	
		}


		else {
			return 0;
		}
	}

	return 0;
}

#endif
