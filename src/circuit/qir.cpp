
#include <fstream>
#include <iostream>
#include "qir.h"
#include <map>

using namespace std;

class QIRCirc {
	private:
		ofstream out;
	public:
		QIRCirc() {
			out.open("out.qir");
			QIRTypes.qir_int = "i64";
			QIRTypes.qir_double = "double";
			QIRTypes.qir_bool = "i1";
			QIRTypes.qir_pauli = "%Pauli";
			QIRTypes.qir_range = "%Range";

		}
		
		~QIRCirc() {
			out.close();
		}

		void add_param(map<string,string>& params, string type, string id) {
			params.insert(pair<string,string>(type, id));			

		}


		// llvm create functions
		void llvm_fstart(string fname, map<string,string> params) {
			out << "define @" << fname << "("; 
			int counter = 0;
			for(auto& i : params) {
				out << i.first << " " << i.second << " ";
				if(counter < params.size()-1) {
					out << ",";
				}
				counter++;
			}

			out << ") {\n";
	
		}

		void llvm_fterm() {
			out << "}\n";
		}


		void h_gate() {
			out << "__qir_hadamard__\n";
		}

};


int main() {
	QIRCirc circ;
	map<string, string> ps;
	circ.add_param(ps,QIRTypes.qir_int, "stuff");
	circ.add_param(ps,QIRTypes.qir_double, "idk");
	circ.add_param(ps,QIRTypes.qir_pauli, "x");
	circ.llvm_fstart("o1", ps);
	circ.h_gate();	
	circ.llvm_fterm();
	return 0;

}
