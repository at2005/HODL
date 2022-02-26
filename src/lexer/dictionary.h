#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <vector>


// define custom class Pair to store key-value pairs for lexical analysis
// the structure is <value,token>
class Pair {
public:
	Pair() {
		// default values
		this->value = "VALUE_NDEF";
		this->token = "TOKEN_NDEF";
	}


	Pair(std::string first, std::string second) {
		// parameterized values
		this->value = first;
		this->token = second;
	}

	const std::string getValue() {
		// if not NULL get value component
		if (this) {
			if (&this->value != NULL) {
				return this->value;
			}
		}

		else {
			// if NULL then return VALUE_NDEF
			return "VALUE_NDEF";
		}
	}

	const std::string getToken() {
		// get token component
		if (this) {
			if (&this->token != NULL) {
				// return token
				return this->token;
			}

			else {
				// else return TOKEN_NDEF
				return "TOKEN_NDEF";
			}
		}

		// default return NULL string
		return "";
	}

	// set token
	void setToken(std::string tokenNew) {
		this->token = tokenNew;
	}


	// set value
	void setValue(std::string valueNew) {
		this->value = valueNew;
	}

private:
	std::string value;
	std::string token;


};


// class that holds <value,token> pairs
class Dict {

	public:
	// vector output containing <value,token> pairs
	std::vector<Pair> dict_output = {};

	// add <token,value>
	void add(std::string val, std::string tok) {
		dict_output.push_back(*(new Pair(val, tok)));
	}

	// for debugging purposes print lexical output
	const void printLex() {
		std::vector<Pair>::iterator iter;
		for (iter = dict_output.begin(); iter != dict_output.end(); ++iter) {
			std::cout << iter->getValue() << " : " << iter->getToken() << std::endl;
		}
	}

};




#endif

