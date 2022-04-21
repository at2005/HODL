#ifndef TYPES_H
#define TYPES_H

#include <string>
#include "../circuit/gates.h"
#include <unordered_map>

// struct holds hash tables that contain the lexemes

struct
{
	unordered_map<string, int> type = {
		{"super" , 1},
		{"int" , 1},
		{"float" , 1}
	};

	unordered_map<string, int> keyword = {
		{"measure", 1},
		{"return", 1}
	};


	unordered_map<string, int> conditional = {
		{"if" , 1},
		{"elsif" , 1},
		{"else", 1}
	};


	unordered_map<string, int> bool_expr = {
		{"&" , 1},
		{"|", 1},
		{"!", 1},
		{"^",1}
	};


	unordered_map<string, int> intrinsic_func = {
		{"filter",1},
		{"qft",1 },
		{"mark",1}
	};


	unordered_map<string, int> inline_asm = {
		{"H",1},
		{"CH",1},
		{"CZ", 1},
		{"CX", 1},
		{ "CRX",1 },
		{"CRY",1},
		{"CRZ",1},
		{"RX",1},
		{"RY",1},
		{"RZ",1},
		{"S",1},
		{"SDG",1},
		{"T",1},
		{"TDG",1},
		{"X",1},
		{"Y",1},
		{"Z",1},
		{"ID",1},
		{"SWAP",1},
		{"CCX",1}
	};

	unordered_map<string, int> func_decl = {
		{"function",1},
		{"oracle", 1}
	};


	unordered_map<string, int> loop = {
		{"for",1},
		{"while",1}
	};
	 
	unordered_map<string, int> op = {
		{"+",1},
		{"-",1},
		{"*",1},
		{"/",1},
		{":",1},
		{"+=",1},
		{"*=",1},
		{"/=",1},
		{"-=",1}
		
	};

	unordered_map<string, int> op_append = {
		{"+=",1},
		{"*=",1},
		{"/=",1},
		{"-=",1}
	};


	unordered_map<string, int> relational_op = {
		{"==",1},
		{"<=",1},
		{">=",1},
		{"!=",1},
		{">",1},
		{"<",1},
		{"!=",1}
	};

	unordered_map<string, int> container = {
		{"{",1},
		{"}",1},
		{"(",1},
		{")",1},
		{"[",1},
		{"]",1}

	};


	unordered_map<string, int> left_parentheses = {
		{"(",1}
	};


	unordered_map<string, int> right_parentheses = {
		{")",1}
	};

	unordered_map<string, int> left_brace = {
		{"{",1}
	};
	unordered_map<string, int> right_brace = {
		{"}",1}
	};

	unordered_map<char, int> charset = {
		{'a',1},
		{'b',1},
		{'c',1},
		{'d',1},
		{'e',1},
		{'f',1},
		{'g',1},
		{'h',1},
		{'i',1},
		{'j',1},
		{'k',1},
		{'l',1},
		{'m',1},
		{'n',1},
		{'o',1},
		{'p',1},
		{'q',1},
		{'r',1},
		{'s',1},
		{'t',1},
		{'u',1},
		{'v',1},
		{'w',1},
		{'x',1},
		{'y',1},
		{'z',1}
	};


	unordered_map<char, int> digit = {
		{'0',1},
		{'1',1},
		{'2',1},
		{'3',1},
		{'4',1},
		{'5',1},
		{'6',1},
		{'7',1},
		{'8',1},
		{'9',1},
		{'.',1}
	};


} lex_states;

#endif
