#ifndef FUNC_TABLE
#define FUNC_TABLE

#include "Function.h"
#include <unordered_map>
//class follows singleton pattern

class FunctionTable {

	FunctionTable() {
		
	}

public:
	static FunctionTable* get_func_table() {
		if (!function_call_table) {
			function_call_table = new FunctionTable();
		}

		return function_call_table;
	}
	

	void push_func(Function* func) {
		this->func_table.insert({ func->get_name(), func });
	}


	//return data from quantum function stack
	Function* search_qfunc_stack(string qfunc_name) {
		return func_table.find(qfunc_name)->second;
	}

	//searches global function stack
	Function* search_global_func_stack(string gfunc_name) {
		return func_table.find(gfunc_name)->second;
	}


	



private:
	std::unordered_map<string, Function*> func_table;
	static FunctionTable* function_call_table;
	

};


FunctionTable* FunctionTable::function_call_table = NULL;

#endif
