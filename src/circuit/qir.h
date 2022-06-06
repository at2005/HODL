#ifndef QIR_H
#define QIR_H

#include <string>
using namespace std;


#include <fstream>
#include <iostream>
#include <map>

class QIRCirc {
	private:
		ofstream out;
		struct {
			string qir_int;
			string qir_double;
			string qir_bool;
			string qir_pauli;
			string qir_range;	


		} QIRTypes;


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

		
		void init_circ() {
			out << "\%Qubit = type opaque\n";
			out << "\%Result = type opaque\n";
			this->llvm_fstart("void", "main",{});

		}


		void add_param(map<string,string>& params, string type, string id) {
			params.insert(pair<string,string>(type, id));			

		}


		// llvm create functions
		void llvm_fstart(string type, string fname, map<string,string> params) {
			out << "define " << type << " @" << fname << "("; 
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
			if(qubit_index == 0) return "null";
			return "inttoptr (" + type + " " + to_string(qubit_index) + " to %Qubit)";

		}

		string index_to_qtype(unsigned int qubit_index) {
			return "%Qubit* " + get_inttoptr(QIRTypes.qir_int, qubit_index);

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


		void s_gate(unsigned int qubit_index) {
			out << "call void @__quantum__qis__s__body( " << index_to_qtype(qubit_index) << ")\n";
		}
		

		void sdg_gate(unsigned int qubit_index) {
			out << "call void @__quantum__qis__sdg__body( " << index_to_qtype(qubit_index) << ")\n";
		}


		void t_gate(unsigned int qubit_index) {
			out << "call void @__quantum__qis__t__body( " << index_to_qtype(qubit_index) << ")\n";
		}


		
		void phase_gate(unsigned int qubit_index, double theta) {
			out << "call void @__quantum__qis__p__body( " << index_to_qtype(qubit_index) << "," << QIRTypes.qir_double << " " << to_string(theta) <<  ")\n";
		}

	

		void cnot_gate(unsigned int control_index, unsigned int target_index) {
			out << "call void @__quantum__qis__cnot__body( " << index_to_qtype(control_index) << "," << index_to_qtype(target_index) << ")\n";
		}

		void cz_gate(unsigned int control_index, unsigned int target_index) {
			out << "call void @__quantum__qis__z_ctl_body( " << index_to_qtype(control_index) << "," << index_to_qtype(target_index) << ")\n";
		}


		void cp_gate(unsigned int control_index, unsigned int target_index, double theta) {
			out << "call void @__quantum__qis__p_ctl__body( " << index_to_qtype(control_index) << "," << index_to_qtype(target_index) << "," << QIRTypes.qir_double << " " << to_string(theta) <<  ")\n";

		}
	
		
		void ccx_gate(unsigned int control_index, unsigned int target1_index, unsigned int target2_index) {
			out << "call void @__quantum__qis__toffoli__body( " << index_to_qtype(control_index) << "," << index_to_qtype(target1_index) << "," << index_to_qtype(target2_index) << ")\n";


		}


		void measure(string res,unsigned int qubit_index) {
			out << res << "= call %Result* @__quantum__qis__m__body( " << index_to_qtype(qubit_index) << ")\n"; 

		}

		void qgate_decl() {

			out << "declare void @__quantum__qis__h__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__z__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__cnot__body(%Qubit*, %Qubit*)" << endl;
			out << "declare %Result* @__quantum__qis__m__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__x__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__y__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__s__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__sdg__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__t__body(%Qubit*)" << endl;
			out << "declare void @__quantum__qis__p__body(%Qubit*, " << QIRTypes.qir_double << ")" << endl;
			out << "declare void @__quantum__qis__cp__body(%Qubit*, %Qubit*, " << QIRTypes.qir_double << ")" << endl;
			out << "declare void @__quantum__qis__cz__body(%Qubit*, %Qubit*)" << endl;
			out << "declare void @__quantum__qis__toffoli_body(%Qubit*, %Qubit*, %Qubit*)";			
			

		}
};


#endif
