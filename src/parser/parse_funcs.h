#ifndef PARSE_FUNCS_H
#define PARSE_FUNCS_H

#include <string.h>
#include <map>
#include "node.h"



using namespace std;

//utility function for returning separated values


vector<vector<Pair>> returnSep(vector<Pair> inputExpr) {
	vector<vector<Pair>> tempVec = {};
	vector<Pair> i_stmnts = {};
	unsigned int paren_counter = 0;
	for (int i = 0; i < inputExpr.size(); i++) {
		if(inputExpr[i].getValue() == "(") paren_counter++;
		else if(inputExpr[i].getValue() == ")") paren_counter--; 
		
		if (inputExpr[i].getToken() == "SEPERATOR" && paren_counter == 0) {
			tempVec.push_back(i_stmnts);
			i_stmnts.clear();
		}

		else if (i == inputExpr.size() - 1) {
			i_stmnts.push_back(inputExpr[i]);
			tempVec.push_back(i_stmnts);
			i_stmnts.clear();

		}

		else {
			i_stmnts.push_back(inputExpr[i]);
		}
	}

	return tempVec;
}


//utility function for parsing between two values of strings
vector<Pair> parse_between(string a, string b, vector<Pair> input) {

	//results stored in temporary vector
	vector<Pair> tempVec = {};
	//variables hold the beginning and ending index respectively. Init to NULL
	int begin = -1;
	int end = -1;
	int count = 0;
	for (int i = 0; i < input.size(); i++) {
		if (input[i].getValue() == a) {
			if (begin == -1) {
				//cout << i << endl;
				begin = i;
			}
			count++;
		}

		else if (input[i].getValue() == b) {
			count--;
			if (count == 0) {

				end = i;
				break;
			}

		}

	}




	//out << begin << " : " << end << endl;
	if (begin != -1 && end != -1) {

		for (int j = begin + 1; j < end; j++) {
			tempVec.push_back(input[j]);
		}
	}

	else {

	}



	return tempVec;

}


 map<string, int> get_precedence_map() {
	map<string, int> precedence_map;

	precedence_map.insert({"^", 2});
	precedence_map.insert({"|", 2});
	precedence_map.insert({ "&" , 2});
	precedence_map.insert({ ":" , 5 });
	precedence_map.insert({ ">" , 5 });
	precedence_map.insert({ "<" , 5 });
	precedence_map.insert({ "==" , 5 });
	precedence_map.insert({ ">=" , 5 });
	precedence_map.insert({ "<=" , 5 });
	precedence_map.insert({ "+=" , 5 });
	precedence_map.insert({ "-=" , 5 });
	precedence_map.insert({ "*=" , 5 });
	precedence_map.insert({ "+" , 10 });
	precedence_map.insert({ "-" , 10 });
	precedence_map.insert({ "|" , 10 });
	precedence_map.insert({ "*" , 20 });
	precedence_map.insert({ "/" , 20 });	

	return precedence_map;
}


//checks for higher precedence between two operators

bool has_higher_precedence(Pair operator1, Pair operator2) {


	if (get_precedence_map().find(operator1.getValue())->second > get_precedence_map().find(operator2.getValue())->second) {
		return true;
	}

	return false;




}









inline Node* parse_maths(vector<Pair> input) {
	//creates a default 'root' node
	Node* root = new Node();
	if (input.size() == 1) {
		return (root = new Node(input[0]));
	}


	else {
		Node* currentParent = root;
		Node* final_root = root;
		Node* tempParent = NULL;
		Pair prev_op;
		bool switchParent = false;
		// has_op is a variable to see if the previous operator has lower precedence than the current one
		bool has_seen_op = false;
		bool has_seen_id = false;

		for (int i = 0; i < input.size(); i++) {
			if (input[i].getToken() == "NUMBER" || input[i].getToken() == "IDENTIFIER" || input[i].getToken() == "KEYWORD" || input[i].getToken() == "BUILT_IN_FUNCTION" || input[i].getToken() == "ASSEMBLY_INSTRUCTION") {
				has_seen_id = true;
				//creates numbers as left and right children of the operator Node
				if (has_seen_op) {

					currentParent->createRightChild(input[i], "R_PARAM");
					if (input[i].getToken() == "NUMBER") {
						currentParent->getRightChild()->set_classical();
					}

					if (!currentParent->getLeftChild()) {
						currentParent->createLeftChild(new Node(Pair("0", "NUMBER")));
						currentParent->getLeftChild()->set_classical();

					}

					if (currentParent->getLeftChild()->is_classical() && currentParent->getRightChild()->is_classical()) {
						currentParent->set_classical();
					}

					if (switchParent) {
						currentParent = tempParent;
						if (currentParent->getLeftChild()->is_classical() && currentParent->getRightChild()->is_classical()) {
							currentParent->set_classical();
						}
					}





				}

				else {
					currentParent->createLeftChild(input[i], "L_PARAM");

					if (input[i].getToken() == "NUMBER") {
						currentParent->getLeftChild()->set_classical();
					}




				}
			}

			else if (input[i].getValue() == "[") {
				try {
					if (has_seen_op) {

						currentParent->getRightChild()->setPurpose(input[i + 1].getValue());
					}

					else {
						currentParent->getLeftChild()->setPurpose(input[i + 1].getValue());

					}

					i += 2;
				}

				catch (...) {
					cerr << "INVALID_SUBSCRIPT_OPERATION";
				}
			}
			//sets the empty operator node to the operator encountered
			else if (input[i].getToken() == "OPERATOR" || input[i].getToken() == "COMPARISON_OPERATOR" || input[i].getToken() == "BOOL_EXPR") {
				has_seen_id = false;
				//if the parser has not encountered an operator it will set the type of the current parental node to this operator type
				if (!has_seen_op) {
					has_seen_op = true;
					//setting the previous operator to this
					prev_op = input[i];
					currentParent->setTreeTV(input[i]);
					currentParent->setPurpose(input[i].getValue());
				}

				
				else if (has_seen_op) {
					Node* np = new Node(input[i], input[i].getValue());
					//checks for higher precedence
					if (has_higher_precedence(input[i], prev_op)) {
						//makes the new node's child the right child of the root node
						np->createLeftChild(currentParent->getRightChild());
						//sets new node to be the right child of the root node
						currentParent->setRightChild(np);
						currentParent->flip_classical();



						//sets current parent to be the new parent node
						tempParent = currentParent;
						currentParent = np;
						switchParent = true;
					}

					else if (!currentParent->getRightChild()) {
						currentParent->createRightChild(np);
						tempParent = currentParent;
						currentParent = np;
						switchParent = true;
					}

					else {
						np->createLeftChild(currentParent);
						currentParent = np;


						final_root = np;



					}


					prev_op = input[i];

				}




			}
			

			//case for expression in parantheses
			else if (input[i].getValue() == "(") {
				if (has_seen_id) {
					Node* nd = nullptr;
					if (has_seen_op) nd = currentParent->getRightChild();
					else nd = currentParent->getLeftChild();
					
					vector<Pair> params = parse_between("(", ")", input);
					for (int j = 0; j < params.size(); j++) {
		
						nd->push_func_param(params[j]);
						
					}

					nd->setTreeToken("FCALL");
					i += nd->get_func_params().size() + 1;

				}
			

				else {
					vector<Pair> parantheses_search_space;
					for (int j = i; j < input.size(); j++) {
						parantheses_search_space.push_back(input[j]);
					}

					if (has_seen_op) {
						currentParent->createRightChild(parse_maths(parse_between("(", ")", parantheses_search_space)));
					}

					else {
						currentParent->createLeftChild(parse_maths(parse_between("(", ")", parantheses_search_space)));

					}

					if (currentParent->getLeftChild()->is_classical() && currentParent->getRightChild()->is_classical()) {
						currentParent->set_classical();
					}


					i += parse_between("(", ")", parantheses_search_space).size() + 1;


				}
			}
		}

		//ensures that no empty node is returned
		int iter = 0;
		while (final_root->getTToken() == "TOKEN_NDEF" && iter < 100) {
			final_root = final_root->getLeftChild();
			iter++;
		}

		Node* testestest = new Node();
		*testestest = *final_root;

		return final_root;
	}


}

#endif
