#pragma once

#include <iostream>
#include <fstream>
#include "../lexer/Lexer.h"
using namespace std;

static std::string target_system = "IBM";
static string target_file;
static string input_file;


Lex execute_lex(string src_file) {


	Lex lx;
	string nameOfFile = src_file;

	
		target_file = "program.qasm";
		target_system = "IBM";

		//nameOfFile += ".q";
		ifstream FileIn(nameOfFile);
		std::string line;

		if (FileIn.is_open()) {
			while (getline(FileIn, line)) {
				lx.lexer(line);
			}

			return lx;
		}

		else {

			FileIn.close();
			system("Color 04");
			cout << "FILE DOES NOT EXIST!\n\n";
			system("Color 0F");
			exit(0);
		}



}


template<typename T>

void print_vector(vector<T> input) {
	for (int i = 0; i < input.size(); i++) {
		cout << input[i].getToken() << " : " << input[i].getValue() << endl;
	}

}
