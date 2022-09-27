#ifndef TOKENS_H
#define TOKENS_H

#include "types.h"



//Function below checks if string is a valid operator. Returns true if it is and false if not.
bool isAlphabet(char input) {
	bool cond1 = lex_states.charset.find(input) != lex_states.charset.end();
	bool cond2 = lex_states.charset.find(toupper(input)) != lex_states.charset.end();

	if (cond1 || cond2) {
		return true;
	}
	return false;
}

bool isOperator(std::string input) {

	if (lex_states.op.find(input) != lex_states.op.end()) {
		return true;
	}

	return false;

}


bool isAppendOperator(std::string input) {
	if (lex_states.op_append.find(input) != lex_states.op_append.end()) {
		return true;
	}

	return false;
}



bool isAssignment(std::string input) {
	bool is_assignment = false;
	for (int i = 0; i < input.size(); i++) {
		if (input[i] == '=') {
			if(!is_assignment) is_assignment = true;
			else { return false; }
		}

		else {
			is_assignment = false;
			break;
		}
	}

	if (is_assignment) return true;
	return false;
}


bool isType(std::string input) {
	if (lex_states.type.find(input) != lex_states.type.end()) {
		return true;
	}

	return false;
}

//Function below checks if string is a keyword. Returns true if it is, otherwise false.

bool isKeyword(std::string input) {

	if (lex_states.keyword.find(input) != lex_states.keyword.end()) {
		return true;
	}

	return false;

}





//Function below returns true if input is a bool operator, e.g., and, or, not.

bool isBoolExpr(std::string input) {
	if (lex_states.bool_expr.find(input) != lex_states.bool_expr.end()) {
		return true;
	}

	return false;
}


bool isNumeric(std::string input);
//checks if variable is an identifier
bool isIdentifier(std::string input) {
	bool is_alpha = false;
	int counter = 0;
	std::string temp = "";
	for (unsigned int j = 0; j < input.size(); j++) {
		string s = ""; s += input[j];
		if (isNumeric(s)) {
			if (is_alpha) counter++;
		}

		if (isAlphabet(input[j]) || input[j] == '_') {
			if (!(isKeyword(input) && isBoolExpr(input))) {
				is_alpha = true;
				counter++;
			}

		}
	}
	if (counter == input.size()) {
		return true;
	}
	else {
		return false;
	}
}

//checks if input string is categorized as a number
bool isNumeric(std::string input) {
	if (input == "pi") return true;
	int counter = 0;
	for (unsigned int i = 0; i <= input.size(); i++) {
		if(lex_states.digit.find(input[i]) != lex_states.digit.end()) {
			counter++;
		}

	}

	if (counter == input.size()) {
		return true;
	}

		return false; 
}

	bool isFloat(std::string input) {
		int counter = 0;
		bool has_point = false;
		for (unsigned int i = 0; i <= input.size(); i++) {
			
			if (lex_states.digit.find(input[i]) != lex_states.digit.end()) {
				counter++;

			}

			if (input[i] == '.') {
				has_point = true;
			}

		}

		

		if (counter == input.size() && has_point) {
			return true;
		}

		return false;
	}



//checks if input is a comparison operator
bool isCompOp(std::string input) {
	if (lex_states.relational_op.find(input) != lex_states.relational_op.end()) {
		return true;
	}

	return false;

}





//The following functions check for braces, curly set-like, and parantheses.
inline bool isInitBlock(std::string input) {
	if (input == "{") {
		return true;
	}

	return false;
}





inline bool isTermBlock(std::string input) {
	if (input == "}") {
		return true;
	}

	return false;
}





inline bool isInitPar(std::string input) {
	if (input == "(") {
		return true;
}

	return false;

}




inline bool isTermPar(std::string input) {
	if (input == ")") {
		return true;
	}
	
	return false;
}

inline bool isInitSquareBracket(std::string input) {
	if (input == "[") {
		return true;
	}
	return false;
}

inline bool isTermSquareBracket(std::string input) {
	if (input == "]") {
		return true;
	}
	return false;
}

bool isBracket(std::string input) {
	if (isInitBlock(input) || isTermBlock(input) || isInitPar(input) || isTermPar(input) || isInitSquareBracket(input) || isTermSquareBracket(input)) {
		return true;
	}

	
	return false;
	
}



//Function below checks if string is a built in function. True if it is, or false otherwise

bool isBuiltInFunction(std::string input) {
	if (lex_states.intrinsic_func.find(input) != lex_states.intrinsic_func.end()) {
			return true;

	}

	
	return false;
}

bool isUserFunction(std::string input) {
	if (lex_states.func_decl.find(input) != lex_states.func_decl.end()) {
		return true;
	}
	return false;
}




inline bool isProgram(std::string input) {
	if (input == "main") {
		return true;
	}

	return false;

}

bool isConditional(std::string input) {
	if (lex_states.conditional.find(input) != lex_states.conditional.end()) {
		return true;
	}
	return false;
}



//Function below checks if string is a loop. True if yes, false otherwise

bool isLoop(std::string input) {
	if (lex_states.loop.find(input) != lex_states.loop.end()) {
		return true;
	}
	return false;
}



//Function below checks for comments
inline bool isComment(std::string input) {
	if (input == "#") {
		return true;
	}
	
	return false;

}



//Function below checks for semicolon

inline bool is_semicolon(std::string input) {
	if (input == ";") {
		return true;
	}

	return false;
}


inline bool isSeperator(std::string input) {
	if (input == ",") {
		return true;
	}
	
	return false;

}

inline bool isAssembly(std::string input) {
	if (lex_states.inline_asm.find(input) != lex_states.inline_asm.end()) {
		return true;
	}

	return false;
}


//Function below returns accurate token
std::string getToken(std::string input) {
	
	try {

		if (is_semicolon(input)) return "TERMINATE";
		if (isProgram(input)) return "INITIATE PROGRAM";
		if (isSeperator(input)) return "SEPERATOR";
		if (isType(input)) return "TYPE";
		if (isKeyword(input)) return "KEYWORD";
		if (isAssembly(input)) return "ASSEMBLY_INSTRUCTION";
		if (isBuiltInFunction(input)) return "BUILT_IN_FUNCTION";
		if (isUserFunction(input)) return "USER_DEF_FUNCTION";
		if (isConditional(input)) return "CONDITIONAL";
		if (isLoop(input)) return "LOOP";
		if (isOperator(input)) return "OPERATOR";
		if (isAssignment(input)) return "ASSIGNMENT";
		if (isBoolExpr(input)) return "BOOL_EXPR";
		if (isIdentifier(input)) return "IDENTIFIER";
		if (isNumeric(input)) return "NUMBER";
		if (isFloat(input)) return "FLOAT";
		if (isCompOp(input)) return "COMPARISON_OPERATOR";
		if (isInitBlock(input)) return "COMMENCE_BLOCK";
		if (isTermBlock(input)) return "TERMINATE_BLOCK";
		if (isInitPar(input)) return "COMMENCE_PARANTHESES";
		if (isTermPar(input)) return "TERMINATE_PARANTHESES";
		if (isInitSquareBracket(input)) return "INITIATE_SQUARE_BRACKETS";
		if (isTermSquareBracket(input)) return "TERMINATE_SQUARE_BRACKET";
		if (isComment(input)) return "COMMENT";
		return "INVALID_TOKEN_ERROR";
	}

	catch (std::string exception) {
		return "INVALID_TOKEN_ERROR";

	}

}

#endif
