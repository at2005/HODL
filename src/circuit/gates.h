#ifndef GATES_H
#define GATES_H

#include <string>
using namespace std;

// INSTRUCTION SET FOR QASM 2.0
namespace QASM{
	constexpr const char* VERSION = "2.0";
	constexpr const char* DEPENDENCY = "\"qelib1.inc\"";
	constexpr const char* HADAMARD = "h";
	constexpr const char* CONTROLLED_NOT = "cx";
	constexpr const char* ID = "id";
	constexpr const char* ROTATE_X = "rx";
	constexpr const char* ROTATE_Y = "ry";
	constexpr const char* ROTATE_Z = "rz";
	constexpr const char* X  = "x";
	constexpr const char* Y = "y";
	constexpr const char* Z = "z";
	constexpr const char* S = "s";
	constexpr const char* SDG = "sdg";
	constexpr const char* T=  "t";
	constexpr const char* TDG = "tdg";
	constexpr const char* U1 = "u1";
	constexpr const char* CU1 =  "cu1";
	constexpr const char* CONTROLLED_HADAMARD = "ch";
	constexpr const char* CONTROLLED_Z = "cz";
	constexpr const char* CONTROLLED_ROTATE_X  ="crx";
	constexpr const char* CONTROLLED_ROTATE_Y=  "cry";
	constexpr const char* CONTROLLED_ROTATE_Z = "crz";
	constexpr const char* TOFFOLI = "ccx";
	constexpr const char* SWAP  ="swap";
	constexpr const char* BARRIER  ="barrier";
}

#endif
