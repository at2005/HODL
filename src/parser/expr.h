#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <string.h>
#include "node.h"

inline string rtnExprType(vector<Pair> input) {

	if (input.size() == 0) {
		return "INVALID";
	}

	else if (input.size() == 1) {
		return "SINGLE";
	}

	else if (input[0].getToken() == "TYPE" && input[1].getToken() == "IDENTIFIER") {

		return "VARIABLE_DECLARATION";
	}

	//CHECKS FOR USER DEFINED FUNCTION
	else if (input[0].getToken() == "USER_DEF_FUNCTION") {
		if (input[0].getValue() == "function") {
			return "FUNCTION_DECLARATION";
		}

		else if (input[0].getValue() == "oracle") {
			return "ORACLE_FUNCTION";
		}


		else {
			return "INVALID SYNTAX";
		}
	}

	else if (input[1].getToken() == "USER_DEF_FUNCTION") {
		return  "FUNCTION_DECLARATION";
	}



	//CHECKS FOR FUNCTION CALL
	else if ((input[0].getToken() == "IDENTIFIER" || input[0].getToken() == "BUILT_IN_FUNCTION" || input[0].getToken() == "ASSEMBLY_INSTRUCTION") && input[1].getToken() == "COMMENCE_PARANTHESES") {
		return "FUNCTION_CALL";
	}



	//CHECKS FOR ASSIGNMENT
	else if (input[0].getToken() == "IDENTIFIER" || input[0].getToken() == "NUMBER" || input[0].getToken() == "COMMENCE_PARANTHESES" || input[0].getToken() == "OPERATOR") {
		return "OPERATION";
	}

	else if (input[0].getToken() == "KEYWORD") {
		return "KEYWORD_OPERATION";
	}

	//CHECKS IF TYPE OF EXPRESSION IS A CONDITIONAL
	else if (input[0].getToken() == "CONDITIONAL") {
		if (input[0].getValue() == "if") {
			return "IF_STATEMENT";
		}

		else if (input[0].getValue() == "elsif") {
			return "ELSE_IF_STATEMENT";
		}

		else if (input[0].getValue() == "else") {
			return "ELSE_STATEMENT";
		}

		else {
			return "INVALID SYNTAX";
		}
	}

	else if (input[0].getToken() == "LOOP") {
		if (input[0].getValue() == "for") {
			return "FOR_LOOP";
		}

		else if (input[0].getValue() == "while") {
			return "WHILE_LOOP";
		}
	}



	//ELSE RETURNS SYNTAX ERROR

	else {
		return "SYNTAX_ERROR";
	}




}





//gets each individual expression

inline vector<vector<Pair>> getEachExpression(vector<Pair> input) {
	int num_paran = 0;

	vector<Pair> tempVec = {};
	vector<vector<Pair>> output = {};
	for (int i = 0; i < input.size(); i++) {

		if (input[i].getToken() == "COMMENCE_BLOCK") {
			num_paran++;
			tempVec.push_back(input[i]);
		}

		else if (input[i].getToken() == "TERMINATE_BLOCK") {
			num_paran--;
			tempVec.push_back(input[i]);

			if (num_paran == 0) {

				output.push_back(tempVec);
				tempVec.clear();
			}
		}

		else if (input[i].getToken() == "TERMINATE" && num_paran == 0) {
			//tempVec.push_back(input[i]);
			output.push_back(tempVec);
			tempVec.clear();

		}

		else {
			tempVec.push_back(input[i]);
		}


	}

	return output;
}



//check if valid expression
inline bool checkIfValidExpr(Pair input) {
	if (input.getToken() == "NUMBER" || input.getToken() == "IDENTIFIER") {
		return true;
	}

	else {
		return false;
	}
}

#endif
