
#include "Tokens.h"
#include "dictionary.h"


class Lex {

private:
	Dict outputLex;
	//create a dictionary
	
public:

	void lexer(std::string line) {
		string prev = "";
		//proxy for storing data while not encountering any other series of chars
		std::string proxy = "";
		std::string tempString = "";
		for (int i = 0; i < line.size(); i++) {
			//assign the first character to tempString
			tempString = line[i];
			// series of checks for lexing

			//semicolon check
			if (is_semicolon(tempString)) {
				if (proxy != "") {
					outputLex.add(proxy, getToken(proxy));

				}
				outputLex.add(tempString, getToken(tempString));
				proxy = "";
			}

			//comment checks
			else if (isComment(tempString)) {
				break;
			}

			//operator checks
			else if (isOperator(tempString) || isCompOp(tempString) || isAssignment(tempString)) {
				char nextStr;
				if (line.size() > i + 1) {
					nextStr = line[i + 1];
				}

				else {
					nextStr = '\0'; 
				}
				//checks for double operators, e.g. <=, ==, ++, etc.
				if (isCompOp(tempString + nextStr) || isOperator(tempString + nextStr)) {
					if (proxy != "") {

						outputLex.add(proxy, getToken(proxy));

					}
					outputLex.add(tempString+nextStr, getToken(tempString+nextStr));
					proxy = "";
					i++;
				}

				else {

					if (proxy != "") {

						outputLex.add(proxy, getToken(proxy));

					}
					outputLex.add(tempString, getToken(tempString));
					proxy = "";
				}
			}


			//checks if bracket or seperator
			else if (isBracket(tempString) || isSeperator(tempString)) {
				if (proxy != "") {
					outputLex.add(proxy, getToken(proxy));

				}

				outputLex.add(tempString, getToken(tempString));
				proxy = "";

			}
			// space and tab checks
			else if (tempString == " " || line[i] == '\t') {
				if (proxy != "") {

					outputLex.add(proxy, getToken(proxy));
					proxy = "";
				}
			}

			//checks for strings
			else {
				if (tempString != " ") {
					proxy += tempString;
				}
			}

			tempString = "";


		}


	}
	
	//gets lexed output 
	const Dict getLex() {
		return outputLex;
	}


};



