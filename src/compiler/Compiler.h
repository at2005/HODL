#ifndef COMPILER_H
#define COMPILER_H

#include "Resources.h"
#include "../general/read.h"
#include "../parser/SyntaxTree.h"
#include "Resources.h"
#include "../circuit/Circuit.h"
#include "../operations/arithmetic.h"
#include "../operations/comparison_operators.h"
#include "../tables/FunctionTable.h"
#include "GenerateInstructions.h"
#include "CompileInstructions.h"


// command line options and their flag
unordered_map<string, bool> options = {
	{"-t", 0}
};




// compile main function
int Compile(int num_args, char** args) {
	// get function table -> for storing function pointers
	FunctionTable* function_table = FunctionTable::get_func_table();
	
	// get program source file from CL
	const char* program_file = args[num_args-1];
	
	// iterate over command line args
	for (int i = 1; i < num_args - 1; i++) {
		// set flag for corresponding option
		options[args[i]] = 1;
	}

		
	//lexical analysis -> stored in vector of token-values
	vector<Pair> TokenValues = execute_lex(program_file).getLex().dict_output;
	
	// get circuit object
	Circuit* qc = Circuit::get_circuit();

	//print_vector(TokenValues);
	//breaks up into individual expressions
	vector<vector<Pair>> expressions = getEachExpression(TokenValues);
	
	
	// main function symbol table
	SymbolTable main_table;

	//declare class instance for main function
	Function MAIN;

	//create Abstract-Syntax-Tree for each function and PUSH into Function stack (function symbol table) if not MAIN
	for (int individual_expressions = 0; individual_expressions < expressions.size(); individual_expressions++) {

		// create AST
		SyntaxTree* AST = new SyntaxTree(expressions[individual_expressions]);
		//cout << expressions[individual_expressions][0].getValue() << endl;
		//cout << AST->getRoot();
		// if not main
		if (AST->getRoot()->getTValue() != "main") {
			// create new function
			Function* func = new Function(AST);

			// set oracle flag if function is an oracle
			if (func->get_syntax_tree()->getName() == "ORACLE") func->set_oracle();

			// add function to function table
			function_table->push_func(func);
		}

		// assign AST to main
		else {
			MAIN = Function(AST);
		}

	}

	// vector of child trees for main function
	vector<SyntaxTree> statements = MAIN.get_syntax_tree()->get_child_trees();

	// vector of INSTRUCTION objects
	vector<INSTRUCTION> instructions = {};

	// iterate over each statement in main
	for (int statement = 0; statement < statements.size(); statement++) {
		// temporary var to store AST
		SyntaxTree& st_ref = statements[statement];
		//cout << st_ref.getRoot()->getTValue();
	//	// evaluate resources on AST
		eval_resources(&st_ref, &main_table);
		//cout << "hi\n";

	//	// iterate over each child tree and evaluate resources for it
	//	// NOTE -> THIS MUST BE CHANGED. IT DOESN'T WORK FOR NESTED EXPRESSIONS!!!!!
		for (int child_statement = 0; child_statement < st_ref.get_child_trees().size(); child_statement++) {
	
			eval_resources(&st_ref.get_child_trees()[child_statement], &main_table);
		}

		
		// create AST 

		create_instructions(instructions, &st_ref, {}, &main_table);
	}
		// If -t (tape) option true,
		// print instruction tape -> debugging tool
		if (options["-t"] == 1) {
			cout << "-------------------\n";
			for (int i = 0; i < instructions.size(); i++) {
				instructions[i].print();
			}

		}

	// iterate over each quantum variable
	for (QuantumVariable*& qvar : qc->get_qvars()) {
		// add quantum register to circuit
		qc->add_qregister(*qvar);
	}


	// iterate over instructions and compile each instruction
	compile_instructions(*qc, instructions, &main_table);

	// delete circuit object
	qc->~Circuit();
		

	return 0;

}


#endif







