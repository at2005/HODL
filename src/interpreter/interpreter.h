#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../parser/SyntaxTree.h"
#include "../lexer/Tokens.h"
#include <string>
#include "../tables/FunctionTable.h"
#include "../tables/SymbolTable.h"
// classical interpreter
long interpret(SyntaxTree* tree, SymbolTable& table, bool in_loop=false) {
	
	// get root of expression tree
	Node* node = tree->getRoot();

	// if root exists
	if (node) {
		SyntaxTree tree_left(node->getLeftChild());
		SyntaxTree tree_right(node->getRightChild());
		// case for root being an assigmnent
		if (node->getTToken() == "ASSIGNMENT") {

			// if the assignment type is classical, e.g. int
			if (node->getLeftChild()->getPurpose() == "int") {
				// get value of right hand side
				long value = interpret(&tree_left, table);

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
			
			// initialize result to zero
			long result = 0;

			// left is the left-hand-child
			string left = node->getLeftChild()->getTValue();

			// right is the right-hand-child
			string right = node->getRightChild()->getTValue();

			// if right node is an identifier
			if (node->getRightChild()->getTToken() == "IDENTIFIER") {
				// check if right node is a classical variable
				if (table.cvar_exists(node->getRightChild()->getTValue())) {
					// set flag to classical if so
					node->getRightChild()->set_classical();
				}
			}

			// if left node is an identifier
			if (node->getLeftChild()->getTToken() == "IDENTIFIER") {
				// check if left node is a classical node
				if (table.cvar_exists(node->getLeftChild()->getTValue())) {
					// set flag to classical if so
					node->getLeftChild()->set_classical();
				}

			}

			//goes into recursion
			if (node->getRightChild()->is_classical() && node->getLeftChild()->is_classical()) {
				// case for addition
				if (node->getTValue() == "+") {
					// set result to sum of left and right side
					result = interpret(&tree_left, table,in_loop) + interpret(&tree_right, table,in_loop);
				}

				
				// case for subtraction
				else if (node->getTValue() == "-") {
					// set result to difference of left and right side
					result = interpret(&tree_left, table,in_loop) - interpret(&tree_right, table,in_loop);
				}

				// case for multiplication
				else if (node->getTValue() == "*") {
					// set result to product of left and right side
					result = interpret(&tree_left, table,in_loop) * interpret(&tree_right, table,in_loop);

				}

				// case for division
				else if (node->getTValue() == "/") {
					// set result to ratio of left and right side
					result = interpret(&tree_left, table,in_loop) / interpret(&tree_right, table,in_loop);

				}

				// case for addition-append
				else if (node->getTValue() == "+=") {
					// if variable on the left is a classical variable
					if (table.cvar_exists(left)) {
						// get classical variable
						ClassicalVariable* cvar = table.search_ctable(left);
						// increment its value by right-hand-side
						cvar->set_value(cvar->get_value() + interpret(&tree_right, table,in_loop));
					}
				}

				// case for subtraction-append
				else if (node->getTValue() == "-=") {
					if (table.cvar_exists(left)) {
						ClassicalVariable* cvar = table.search_ctable(left);
						cvar->set_value(cvar->get_value() - interpret(&tree_right, table,in_loop));
					}
				}

				else if (node->getTValue() == "*=") {
					if (table.cvar_exists(left)) {
						ClassicalVariable* cvar = table.search_ctable(left);
						cvar->set_value(cvar->get_value() * interpret(&tree_right, table,in_loop));
					}
				}

				else if (node->getTValue() == "/=") {
					if (table.cvar_exists(left)) {
						ClassicalVariable* cvar = table.search_ctable(left);
						cvar->set_value(cvar->get_value() / interpret(&tree_right, table,in_loop));
					}
				}

				else if (node->getTValue() == ">") {
					result = interpret(&tree_left, table, in_loop) > interpret(&tree_right, table,in_loop);
				}

				else if (node->getTValue() == ">=") {
					result = interpret(&tree_left, table, in_loop) >= interpret(&tree_right, table, in_loop);
				}

				else if (node->getTValue() == "<") {
					result = interpret(&tree_left, table, in_loop) < interpret(&tree_right, table, in_loop);
				}

				else if (node->getTValue() == "<=") {
					result = interpret(&tree_left, table, in_loop) <= interpret(&tree_right, table, in_loop);
				}

				else if (node->getTValue() == "==") {
					result = interpret(&tree_left, table, in_loop) == interpret(&tree_right, table, in_loop);
				}

				else if (node->getTValue() == "!=") {
					result = interpret(&tree_left, table, in_loop) != interpret(&tree_right, table, in_loop);
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
				result = interpret(&tree_right, table);
				if (!in_loop) {
					*node->getRightChild() = Node(Pair(to_string(result), "NUMBER"));
				}
				return result;
			}

			else if (node->getLeftChild()->is_classical()) {
				result = interpret(&tree_left, table);
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
		}

		else if (node->getTValue() == "for") {
			interpret(&tree->get_function_parameters()[0], table,true);
			//cout << interpret(tree.get_function_parameters()[1], table, true);
			vector<SyntaxTree> child_trees = {};
			while (interpret(&tree->get_function_parameters()[1], table, true)) {
				for (int i = 0; i < tree->get_child_trees().size(); i++) {
					//cout << "hi";
					interpret(&tree->get_child_trees()[i], table,true);
					child_trees.push_back(tree->get_child_trees()[i]);

				}

				interpret(&tree->get_function_parameters()[2], table, true);
				
			}
			
			tree->set_child_trees(child_trees);
			cout << &tree << endl;;

		}

		else {
			return 0;
		}
	}

	return 0;
}

#endif
