


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




