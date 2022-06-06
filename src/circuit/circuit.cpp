#ifndef CIRCUIT_CPP
#define CIRCUIT_CPP

#include "circuit.h"

Circuit* Circuit::circuit;


//constructor

Circuit::Circuit(string target_file, string target_system) {
	this->num_qubits_measured = 0;
	qregs = {};
	this->total_qubits = 0;
	this->qreg_map = {}; 
	if(target_system == "qasm") {

		this->system = "IBM";
		this->qubits = 0;
	//	output_file.open("program.qasm");
		output_file.open(target_file);
		output_file << "OPENQASM " << QASM::VERSION << ";" << endl;
		output_file << "include " << QASM::DEPENDENCY << ";" << endl;
	}

	else if(target_system == "qir") {
		this->system = "QIR";
		qirc.init_circ();
		//this->output_file = qirc.get_output(); 	
	} 
}


// constructor
Circuit::Circuit(Circuit& circ) {
	qregs = {};
	this->qubits = circ.get_num_qubits();
	this->num_qubits_measured = circ.num_qubits_measured;
	this->system = circ.system;
}


// destructor closes QASM file
Circuit::~Circuit() {
	output_file.close();
}

// get number of qubits in circuit
const int Circuit::get_num_qubits() {
	return qubits;
}

// increment number of qubits in circuit
void Circuit::inc_num_qubits() {
	this->qubits++;
}


unsigned int Circuit::true_index(string qreg, unsigned int qubit_offset/*, SymbolTable* table*/) {
	// get quantum variable object and check if it is recycled
	QuantumVariable* qreg_in = this->qreg_map[qreg];
	if(qreg_in->is_composite()) {
		// get absolute qubit index
		pair<string,int> new_val = qreg_in->search_composite(qubit_offset);
		qreg_in = this->qreg_map[new_val.first];
		qubit_offset = new_val.second;
		
	}
	
	return qreg_in->get_phys_start() + qubit_offset;		

}


pair<string, int> Circuit::true_reg_index(string qreg, unsigned int qubit_offset) {
	// get real qreg, index pair	
	QuantumVariable* qreg_in = this->qreg_map[qreg];
	if(qreg_in->is_composite()) {
		pair<string,int> new_val = qreg_in->search_composite(qubit_offset);
		return new_val;	
	}
	
	return {qreg, qubit_offset};

}

// add quantum register 
void Circuit::add_qregister(QuantumVariable& qvar) {
	// iterate over each quantum register
	if(qreg_map.find(qvar.get_qreg()) != qreg_map.end()) {
		return;

	}
	// else add register to QASM file
	output_file << "qreg " << qvar.get_qreg() << "[" << qvar.get_num_qubits() << "];\n";
	
	total_qubits += qvar.get_num_qubits();
	// increment total number of registers
	total_registers++;
	// add register to circuit object
	qreg_map.insert({qvar.get_qreg(), &qvar});


}


// add classical register
void Circuit::add_cregister(string creg, int measurable_qubits) {
	// increment number of qubits measured
	this->num_qubits_measured += measurable_qubits;

	// add CREG to QASM file
	output_file << "creg " << creg << "[" << measurable_qubits << "];\n";

}


// ID GATE for custom register
const void Circuit::id(string qreg, unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		// write ID gate to file -> fixed is due to large numbers being represented in scientific notation
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::ID << " " << qreg_index.first <<"[" << qreg_index.second << "];\n";
	}

}

// ID GATE on entire register
const void Circuit::id(string qreg) {
	// if system is IBM
	if (system == "IBM") {
		// write ID gate to file
		output_file << QASM::ID << " " << qreg << ";\n";
	}

}




// H GATE on qubit index for custom register
const void Circuit::h(string qreg, unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		// write H gate to file -> fixed is due to large numbers being represented in scientific notation
		output_file << fixed << QASM::HADAMARD << " " << qreg_index.first << "[" << qreg_index.second << "];\n";
	}

	else if(system == "QIR") {
		qirc.h_gate(true_index(qreg, qubit_index));

	}
}


// H GATE on entire custom register
const void Circuit::h(string qreg) {
	// if system is IBM
	if (system == "IBM") {
		// write H gate to file
		output_file << QASM::HADAMARD << " " << qreg << ";\n";
	}

	else if(system == "QIR") {
		QuantumVariable* qreg_obj = qreg_map[qreg];
		for(int i = 0; i < qreg_obj->get_num_qubits(); i++) {	
			qirc.h_gate(qreg_obj->get_phys_start() + i);

		}	

	}
}




// X GATE on qubit index for custom register
const void Circuit::x(string qreg, unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {

		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		// write X gate to file -> fixed is due to large numbers being represented in scientific notation
		output_file << fixed << QASM::X << " " << qreg << "[" << qreg_index.second << "];\n";
	}
	else if(system == "QIR") {
		
		qirc.x_gate(true_index(qreg, qubit_index));

	}
}

// X GATE on entire custom register
const void Circuit::x(string qreg) {
	// if system is IBM
	if (system == "IBM") {
		// write X gate to file
		output_file << QASM::X << " " << qreg <<";\n";
	}
	else if(system == "QIR") {
					
		QuantumVariable* qreg_obj = qreg_map[qreg];
		for(int i = 0; i < qreg_obj->get_num_qubits(); i++) {	
			qirc.x_gate(qreg_obj->get_phys_start() + i);

		}	

	}
}


// JUST REPEAT THE ABOVE FOR THE FOLLOWING, AND REPLACE THE GATE NAME. TOO LAZY TO BE REPETITIVE


const void Circuit::y(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {

		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::Y << qreg_index.first <<" [" << qreg_index.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.y_gate(true_index(qreg, qubit_index));

	}
}


const void Circuit::y(string qreg) {
	if (system == "IBM") {
		output_file << fixed << QASM::Y << " " << qreg << ";\n";
	}
	else if(system == "QIR") {
		
		QuantumVariable* qreg_obj = qreg_map[qreg];
		for(int i = 0; i < qreg_obj->get_num_qubits(); i++) {	
			qirc.y_gate(qreg_obj->get_phys_start() + i);

		}	

	}
}




//Z


const void Circuit::z(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::Z << qreg_index.first <<" [" << qreg_index.second << "];\n";
	}
	else if(system == "QIR") {
		
		qirc.z_gate(true_index(qreg, qubit_index));

	}
}

const void Circuit::z(string qreg) {
	if (system == "IBM") {
		output_file << fixed << QASM::Z << " " << qreg << ";\n";
	}
	else if(system == "QIR") {
		
		QuantumVariable* qreg_obj = qreg_map[qreg];
		for(int i = 0; i < qreg_obj->get_num_qubits(); i++) {	
			qirc.z_gate(qreg_obj->get_phys_start() + i);

		}	

	}
}





//S
const void Circuit::s(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::S << qreg_index.first <<" [" << qreg_index.second << "];\n";
	}
	else if(system == "QIR") {
		
		qirc.s_gate(true_index(qreg, qubit_index));

	}
}

const void Circuit::s(string qreg) {
	if (system == "IBM") {
		output_file << fixed << QASM::S << " " << qreg << ";\n";
	}
	else if(system == "QIR") {
		
		QuantumVariable* qreg_obj = qreg_map[qreg];
		for(int i = 0; i < qreg_obj->get_num_qubits(); i++) {	
			qirc.s_gate(qreg_obj->get_phys_start() + i);

		}	

	}
}






//SDG

const void Circuit::sdg(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::SDG << qreg_index.first <<" [" << qreg_index.second << "];\n";
	}
	else if(system == "QIR") {
		
		qirc.sdg_gate(true_index(qreg, qubit_index));

	}
}

const void Circuit::sdg(string qreg) {
	if (system == "IBM") {
		output_file << fixed << QASM::SDG << " " << qreg << ";\n";
	}
	else if(system == "QIR") {
		
		QuantumVariable* qreg_obj = qreg_map[qreg];
		for(int i = 0; i < qreg_obj->get_num_qubits(); i++) {	
			qirc.sdg_gate(qreg_obj->get_phys_start() + i);

		}	

	}
}




//T

const void Circuit::t(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::T << qreg_index.first <<" [" << qreg_index.second << "];\n";
	}
	else if(system == "QIR") {
		
		qirc.t_gate(true_index(qreg, qubit_index));

	}
}

const void Circuit::t(string qreg) {
	if (system == "IBM") {
		output_file << fixed << QASM::T << " " << qreg << ";\n";
	}
	else if(system == "QIR") {
		
		QuantumVariable* qreg_obj = qreg_map[qreg];
		for(int i = 0; i < qreg_obj->get_num_qubits(); i++) {	
			qirc.t_gate(qreg_obj->get_phys_start() + i);

		}	

	}
}



//RX

const void Circuit::rx(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::ROTATE_X << "(" << angle_in_radians << ")" << qreg_index.first <<"[ << qreg_index.second << ];\n";
	}
	else if(system == "QIR") {
		

	}
}

const void Circuit::rx(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		output_file << fixed << QASM::ROTATE_X << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
	else if(system == "QIR") {
		

	}
}





//RY

const void Circuit::ry(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::ROTATE_Y << "(" << angle_in_radians << ")" << qreg_index.first <<"[ << qreg_index.second << ];\n";
	}
	else if(system == "QIR") {
		

	}
}

const void Circuit::ry(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		output_file << fixed << QASM::ROTATE_Y << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
	else if(system == "QIR") {
		

	}
}




//RZ

const void Circuit::rz(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::ROTATE_Z << "(" << angle_in_radians << ")" << qreg_index.first <<"[ << qreg_index.second << ];\n";
	}
	else if(system == "QIR") {
		

	}
}

const void Circuit::rz(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		output_file << fixed << QASM::ROTATE_Z << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
	else if(system == "QIR") {
		

	}
}






//U

//WITH DOUBLE AS THETA

const void Circuit::u(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		pair<string,int> qreg_index = true_reg_index(qreg,qubit_index);
		output_file << fixed << QASM::U1 << "(" << angle_in_radians << ") " << qreg_index.first <<"[" << qreg_index.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.phase_gate(true_index(qreg, qubit_index), angle_in_radians);
	}
}

const void Circuit::u(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		output_file << fixed << QASM::U1 << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
	else if(system == "QIR") {
		for(int i = 0; i < qreg_map[qreg]->get_num_qubits(); i++) {	
			qirc.phase_gate(qreg_map[qreg]->get_phys_start() + i, angle_in_radians);
		}

	}
}



//CU
// case where both qubits are in the same register
const void Circuit::cu(string qreg, unsigned int control, unsigned int target, double angle_in_radians) {
	if (system == "IBM") {
		pair<string,int> real_control, real_target;
		real_control = true_reg_index(qreg, control);
		real_target = true_reg_index(qreg, target);
		output_file << fixed << QASM::CU1 << "(" << angle_in_radians << ") " << real_control.first <<"[" << real_control.second << "], " << real_target.first <<"[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.cp_gate(true_index(qreg, control), true_index(qreg, target), angle_in_radians);

	}
}


const void Circuit::cu(string qreg1, unsigned int control, string qreg2, unsigned int target, double angle_in_radians) {
	if (system == "IBM") {
		pair<string,int> real_control, real_target;
		real_control = true_reg_index(qreg1, control);
		real_target = true_reg_index(qreg2, target);
				
		output_file << fixed << QASM::CU1 << "(" << angle_in_radians << ") " << real_control.first << "[" << real_control.second << "], " << real_target.first << "[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.cp_gate(true_index(qreg1, control), true_index(qreg2, target), angle_in_radians);

	}
}



//CX


void Circuit::cx(string qreg, unsigned int control, unsigned int target) {

	if (system == "IBM") {
		pair<string,int> real_control, real_target;
		real_control = true_reg_index(qreg, control);
		real_target = true_reg_index(qreg, target);

		output_file << QASM::CONTROLLED_NOT << " " << real_control.first <<" [" << real_control.second << "]" << " , " << real_target.first <<"[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.cnot_gate(true_index(qreg, control), true_index(qreg, target));

	}
}

void Circuit::cx(string qreg1, unsigned int control, string qreg2, unsigned int target) {
	if (system == "IBM") {
		pair<string,int> real_control, real_target;
		real_control = true_reg_index(qreg1, control);
		real_target = true_reg_index(qreg2, target);
		output_file << QASM::CONTROLLED_NOT << " " << real_control.first << " [" << real_control.second << "]" << " , " << real_target.first << "[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.cnot_gate(true_index(qreg1, control), true_index(qreg2, target));

	}
}

void Circuit::cx(string qreg1, string qreg2) {
	if (system == "IBM") {
		output_file << QASM::CONTROLLED_NOT << " " << qreg1 << "," << qreg2 << ";\n";
	}
	else if(system == "QIR") {
		
	}
}



//CZ

void Circuit::cz(string qreg, unsigned int control, unsigned int target) {
	if (system == "IBM") {
		pair<string,int> real_control, real_target;
		real_control = true_reg_index(qreg, control);
		real_target = true_reg_index(qreg, target);
		output_file << QASM::CONTROLLED_Z << " " << real_control.first << "[" << real_control.second << "]" << " , " << real_target.first << "[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.cz_gate(true_index(qreg, control), true_index(qreg, target));

	}
}


void Circuit::cz(string qreg1, unsigned int control, string qreg2, unsigned int target) {
	if (system == "IBM") {
		pair<string,int> real_control, real_target;
		real_control = true_reg_index(qreg1, control);
		real_target = true_reg_index(qreg2, target);
		output_file << QASM::CONTROLLED_Z << " " << real_control.first << "[" << real_control.second << "]" << " , " << real_target.first << "[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.cz_gate(true_index(qreg1, control), true_index(qreg2, target));
		

	}
}

void Circuit::cz(string qreg1, string qreg2) {
	if (system == "IBM") {
		output_file << QASM::CONTROLLED_Z << " " << qreg1 << " , " << qreg2 << ";\n";
	}
	else if(system == "QIR") {
		

	}
}


//CH

void Circuit::ch(string qreg, unsigned int control, unsigned int target) {
	if (system == "IBM") {
		output_file << QASM::CONTROLLED_HADAMARD << " " << qreg << "[" << control << "]" << " , " << qreg << "[" << target << "];\n";
	}
	else if(system == "QIR") {
		

	}
}

void Circuit::ch(string qreg1, unsigned int control, string qreg2, unsigned int target) {
	if (system == "IBM") {
		output_file << QASM::CONTROLLED_HADAMARD << " " << qreg1 << "[" << control << "]" << " , " << qreg2 << "[" << target << "];\n";
	}
	else if(system == "QIR") {
		

	}
}



//CCX

void Circuit::ccx(string qreg, unsigned int control1, unsigned int control2, unsigned int target) {
	if (system == "IBM") {
		pair<string,int> real_control1, real_control2, real_target;
		real_control1 = true_reg_index(qreg, control1);
		real_control2 = true_reg_index(qreg, control2);
		real_target = true_reg_index(qreg, target);

		output_file << QASM::TOFFOLI << " " << real_control1.first << "[" << real_control1.second << "]," << real_control2.first << "[" << real_control2.second << "]," << real_target.first << "[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.ccx_gate(true_index(qreg, control1), true_index(qreg, control2), true_index(qreg, target));

	}

}

void Circuit::ccx(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target) {
	if (system == "IBM") {
		pair<string,int> real_control1, real_control2, real_target;
		real_control1 = true_reg_index(qreg1, control1);
		real_control2 = true_reg_index(qreg2, control2);
		real_target = true_reg_index(qreg3, target);
		output_file << QASM::TOFFOLI << " " << real_control1.first << "[" << real_control1.second << "]," << real_control2.first << "[" << real_control2.second << "]," << real_target.first << "[" << real_target.second << "];\n";
	}
	else if(system == "QIR") {
		qirc.ccx_gate(true_index(qreg1, control1), true_index(qreg2, control2), true_index(qreg3, target));

	}

}






//CCU1


void Circuit::ccu1(string qreg, unsigned int control1, unsigned int control2, unsigned int target, double theta) {
	this->cu(qreg, control2, target, theta/2);
	this->cx(qreg, control1, control2);
	this->cu(qreg, control2, target, -1*theta/2);
	this->cx(qreg, control1, control2);
	this->cu(qreg, control1, target, theta/2);
		


}




void Circuit::ccu1(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, double theta) {
	this->cu(qreg2, control2, qreg3, target, theta / 2);
	this->cx(qreg1, control1, qreg2, control2);
	this->cu(qreg2, control2, qreg3, target, -theta / 2);
	this->cx(qreg1, control1, qreg2, control2);
	this->cu(qreg1, control1, qreg3, target, theta / 2);
}


//any arbitrary 3 qubit controlled rotation
void Circuit::ccgate(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, int (*gate)(string qreg_func1, unsigned int qubit1, string qreg_func2, unsigned int qubit2)) {
	(*gate) (qreg2, control2, qreg3, target);
	this->cx(qreg1, control1, qreg2, control2);
	(*gate) (qreg2, control2, qreg3, target);
	this->cx(qreg1, control1, qreg2, control2);
	(*gate) (qreg1, control1, qreg3, target);

}


//for single register only
void Circuit::ccgate(string qreg, unsigned int control1, unsigned int control2, unsigned int target, int (*gate)(string qreg_func, unsigned int qubit1, unsigned int qubit2)) {
	(*gate) (qreg, control2, target);
	this->cx(qreg, control1, control2);
	(*gate) (qreg, control2, target);
	this->cx(qreg, control1, control2);
	(*gate) (qreg, control1, target);

}



//for parameterized gates with multiple quantum registers
void Circuit::ccgate(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, string theta, int (*gate)(string qreg_func1, unsigned int qubit1, string qreg_func2, unsigned int qubit2, string theta)) {
		(*gate) (qreg2, control2, qreg3, target, theta);
		this->cx(qreg1, control1, qreg2, control2);
		(*gate) (qreg2, control2, qreg3, target, theta);
		this->cx(qreg1, control1, qreg2, control2);
		(*gate) (qreg1, control1, qreg3, target, theta);

}



//for parameterized gates with single quantum register
void Circuit::ccgate(string qreg, unsigned int control1, unsigned int control2, unsigned int target, string theta, int (*gate)(string qreg_func, unsigned int qubit1, unsigned int qubit2, string theta_func)) {
		(*gate) (qreg, control2, target, theta);
		this->cx(qreg, control1, control2);
		(*gate) (qreg, control2, target, theta);
		this->cx(qreg, control1, control2);
		(*gate) (qreg, control1, target, theta);
	
}



//barrier
const void Circuit::barrier(string qreg) {
	output_file << QASM::BARRIER << " " << qreg << ";\n";


}

#endif
