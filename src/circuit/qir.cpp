
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


		string get_inttoptr(string type, unsigned int qubit_index) {
			return "inttoptr (" << type << " " << qubit_index << "to %Qubit)";

		}

		string index_to_qtype(unsigned int qubit_index) {
			return "%Qubit* " << get_inttoptr(QIRTypes.qir_int, qubit_index);

		}

		void h_gate(unsigned int qubit_index) {
			out << "call void @__quantum__qis__h__body( " << index_to_qtype(qubit_index) << ")\n";
		}

		
		void x_gate(unsigned int qubit_index) {
			out << "call void @__quantum__qis__x__body( " << index_to_qtype(qubit_index) << ")\n";
		}

		
		void y_gate(unsigned int qubit_index) {
			out << "call void @__quantum__qis__y__body( " << index_to_qtype(qubit_index) << ")\n";
		}
		
		void z_gate(unsigned int qubit_index) {
			out << "call void @__quantum__qis__z__body( " << index_to_qtype(qubit_index) << ")\n";
		}


		void cnot_gate(unsigned int control_index, unsigned int target_index) {
			out << "call void @__quantum__qis__cnot__body( " << index_to_qtype(control_index) << "," << index_to_qtype(target_index)")\n";
		}

		
		void measure(string res,unsigned int qubit_index) {
			out << res << "= call %Result* @__quantum__qis__m__body( " << index_to_qtype(control_index) << ")\n"; 

		}

};


int main() {
	QIRCirc circ;
	map<string, string> ps;
	circ.add_param(ps,QIRTypes.qir_int, "stuff");
	circ.add_param(ps,QIRTypes.qir_double, "idk");
	circ.add_param(ps,QIRTypes.qir_pauli, "x");
	circ.llvm_fstart("o1", ps);
	circ.h_gate(0);
	circ.cnot_gate(0,1);	
	circ.measure("%result0", 0);
	circ.measure("%result1", 1);
	circ.llvm_fterm();
	return 0;

}




