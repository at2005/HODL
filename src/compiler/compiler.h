#ifndef COMPILER_H
#define COMPILER_H

#include "../general/read.h"
#include "../general/debug.h"
#include "../parser/ast.h"
#include "resources.h"
#include "../circuit/circuit.h"
#include "../operations/arithmetic.h"
#include "../operations/comparison_operators.h"
#include "../tables/ftable.h"
#include "generate_instructions.h"
#include "compile_instructions.h"


// command line options and their flag
unordered_map<string, bool> options = {
	{"-t", 0},
	{"-o", 0},
	{"-n", 0},
	{"--ayush", 0},
	{"-h",0},
	{"--target",0}
};




// this function compiles the instructions and generates the output file
// compile main function
int compile(int num_args, char** args) {
	// get function table -> for storing function pointers
	FunctionTable* function_table = FunctionTable::get_func_table();

	// get program source file from CL
	const char* program_file = args[num_args-1];
	target_file = "out";	
	// iterate over command line args
	for (int i = 1; i < num_args; i++) {
		// set flag for corresponding option
		options[args[i]] = 1;
		if(!strcmp(args[i], "-o")) {
			target_file = args[i+1];
		}

		if(!strcmp(args[i],"-h")) {
			
			print_help_menu();
			exit(0);
		}

		else if(!strcmp(args[i], "--ayush")) {
			print_debug_info();
			exit(0);
		}

		else if(!strcmp(args[i], "--target")) {
			target_system = args[i+1];

		}

	}

		
	//lexical analysis -> stored in vector of token-values
	vector<Pair> TokenValues = execute_lex(program_file).get_lex().dict_output;
	// get circuit object
	Circuit* qc = Circuit::get_circuit(target_file, target_system);

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
		// evaluate resources on AST
		eval_resources(&st_ref, &main_table);
		
		// iterate over each child tree and evaluate resources for it
		// NOTE -> THIS MUST BE CHANGED. IT DOESN'T WORK FOR NESTED EXPRESSIONS!!!!!
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


	// iterate over each quantum variable - gets rid of duplicates
	int tqubits = 0;
	for (QuantumVariable*& qvar : qc->get_qvars()) {
		// add quantum register to circuit
		qc->add_qregister(*qvar);
		if(target_system == "qir") {
			qvar->set_phys_start(tqubits);
			tqubits += qvar->get_num_qubits();
		}
	}

	
	

	// iterate over instructions and compile each instruction
	compile_instructions(*qc, instructions, &main_table);

	if(target_system == "qir") {
		qc->qirc.llvm_fterm();
		qc->qirc.qgate_decl();

	}



	if(options["-n"]) {
		cout << "logical qubit count: " << qc->total_qubits << endl;
	}	
	// delete circuit object
	qc->~Circuit();
		

	return 0;

}


#endif







