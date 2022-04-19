#ifndef READ_H
#define READ_H

#include <iostream>
#include <fstream>
#include "../lexer/lexer.h"
using namespace std;

static std::string target_system = "IBM";
static string target_file;
static string input_file;


Lex execute_lex(string src_file) {


	Lex lx;
	string nameOfFile = src_file;

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
			cout << "FILE DOES NOT EXIST!\n\n";
			exit(0);
		}



}


void print_help_menu() {
	cout << "\nNice. Thanks for installing HODL:)\nAnyways, here are all the functions:\n\n";
	cout << "\t-o option: specify output file\n";
	cout << "\t-t option: print instruction tape representation of program\n";
	cout << "\t-n option: print number of logical qubits used by program\n";
	cout << "\t-h option: this!\n";
	cout << "See if you can find any Easter Eggs :)\n";


}



#endif
