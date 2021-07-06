#ifndef CIRCUIT_CPP
#define CIRCUIT_CPP

#include "Circuit.h"

Circuit* Circuit::circuit;

//constructor

Circuit::Circuit(string target_file) : num_qubits_measured(0) {
	qregs = {};
	this->system = "IBM";
	this->qubits = 0;
	//outputQASM.open("program.qasm");
	outputQASM.open(target_file);
	outputQASM << "OPENQASM " << QASM::VERSION << ";" << endl;
	outputQASM << "include " << QASM::DEPENDENCY << ";" << endl;

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
	outputQASM.close();
}

// get number of qubits in circuit
const int Circuit::get_num_qubits() {
	return qubits;
}

// increment number of qubits in circuit
void Circuit::inc_num_qubits() {
	this->qubits++;
}


// add quantum register 
void Circuit::add_qregister(QuantumVariable& qvar) {
	// iterate over each quantum register
	for (int i = 0; i < current_qregs.size(); i++) {
		// if qvar exists already
		if (qvar.get_qreg() == current_qregs[i]) {
			// return
			return;
		}
	}

	// else add register to QASM file
	outputQASM << "qreg " << qvar.get_qreg() << "[" << qvar.get_num_qubits() << "];\n";

	// increment total number of registers
	total_registers++;
	
	// add register to circuit object
	current_qregs.push_back(qvar.get_qreg());

}


// add classical register
void Circuit::add_cregister(string creg, int measurable_qubits) {
	// increment number of qubits measured
	this->num_qubits_measured += measurable_qubits;

	// add CREG to QASM file
	outputQASM << "creg " << creg << "[" << measurable_qubits << "];\n";

}


// ID GATE for default register
const void Circuit::id(unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		// write ID gate to file -> fixed is due to large numbers being represented in scientific notation
		outputQASM << fixed << QASM::ID << " qr[" << qubit_index << "];\n";
	}

}


// ID GATE for custom register
const void Circuit::id(string qreg, unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		// write ID gate to file -> fixed is due to large numbers being represented in scientific notation
		outputQASM << fixed << QASM::ID << " " << qreg << "[" << qubit_index << "];\n";
	}

}

// ID GATE on entire register
const void Circuit::id(string qreg) {
	// if system is IBM
	if (system == "IBM") {
		// write ID gate to file
		outputQASM << QASM::ID << " " << qreg << ";\n";
	}

}



// H GATE on qubit index for default register
const void Circuit::h(unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		// write H gate to file -> fixed is due to large numbers being represented in scientific notation
		outputQASM << fixed << QASM::HADAMARD << " qr[" << qubit_index << "];\n";
	}
}


// H GATE on qubit index for custom register
const void Circuit::h(string qreg, unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		// write H gate to file -> fixed is due to large numbers being represented in scientific notation
		outputQASM << fixed << QASM::HADAMARD << " " << qreg << "[" << qubit_index << "];\n";
	}
}


// H GATE on entire custom register
const void Circuit::h(string qreg) {
	// if system is IBM
	if (system == "IBM") {
		// write H gate to file
		outputQASM << QASM::HADAMARD << " " << qreg << ";\n";
	}
}


// X GATE on qubit index for default register
const void Circuit::x(unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		// write X gate to file -> fixed is due to large numbers being represented in scientific notation
		outputQASM << fixed << QASM::X << " qr[" << qubit_index << "];\n";
	}
}


// X GATE on qubit index for custom register
const void Circuit::x(string qreg, unsigned int qubit_index) {
	// if system is IBM
	if (system == "IBM") {
		// write X gate to file -> fixed is due to large numbers being represented in scientific notation
		outputQASM << fixed << QASM::X << " " << qreg << "[" << qubit_index << "];\n";
	}
}

// X GATE on entire custom register
const void Circuit::x(string qreg) {
	// if system is IBM
	if (system == "IBM") {
		// write X gate to file
		outputQASM << QASM::X << " " << qreg << ";\n";
	}
}


// JUST REPEAT THE ABOVE FOR THE FOLLOWING, AND REPLACE THE GATE NAME. TOO LAZY TO BE REPETITIVE

const void Circuit::y(unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::Y << " qr[" << qubit_index << "];\n";
	}
}

const void Circuit::y(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::Y << qreg << " [" << qubit_index << "];\n";
	}
}


const void Circuit::y(string qreg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::Y << " " << qreg << ";\n";
	}
}




//Z

const void Circuit::z(unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::Z << " qr[" << qubit_index << "];\n";
	}
}

const void Circuit::z(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::Z << qreg << " [" << qubit_index << "];\n";
	}
}

const void Circuit::z(string qreg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::Z << " " << qreg << ";\n";
	}
}





//S
const void Circuit::s(unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::S << " qr[" << qubit_index << "];\n";
	}
}

const void Circuit::s(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::S << qreg << " [" << qubit_index << "];\n";
	}
}

const void Circuit::s(string qreg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::S << " " << qreg << ";\n";
	}
}






//SDG
const void Circuit::sdg(unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::SDG << " qr[" << qubit_index << "];\n";
	}
}

const void Circuit::sdg(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::SDG << qreg << " [" << qubit_index << "];\n";
	}
}

const void Circuit::sdg(string qreg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::SDG << " " << qreg << ";\n";
	}
}




//T
const void Circuit::t(unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::T << " qr[" << qubit_index << "];\n";
	}
}

const void Circuit::t(string qreg, unsigned int qubit_index) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::T << qreg << " [" << qubit_index << "];\n";
	}
}

const void Circuit::t(string qreg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::T << " " << qreg << ";\n";
	}
}



//RX
const void Circuit::rx(unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_X << "(" << angle_in_radians << ") qr[" << qubit_index << "];\n";
	}
}

const void Circuit::rx(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_X << "(" << angle_in_radians << ")" << qreg << "[ << qubit_index << ];\n";
	}
}

const void Circuit::rx(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_X << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
}





//RY
const void Circuit::ry(unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_Y << "(" << angle_in_radians << ") qr[" << qubit_index << "];\n";
	}
}

const void Circuit::ry(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_Y << "(" << angle_in_radians << ")" << qreg << "[ << qubit_index << ];\n";
	}
}

const void Circuit::ry(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_Y << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
}




//RZ
const void Circuit::rz(unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_Z << "(" << angle_in_radians << ") qr[" << qubit_index << "];\n";
	}
}

const void Circuit::rz(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_Z << "(" << angle_in_radians << ")" << qreg << "[ << qubit_index << ];\n";
	}
}

const void Circuit::rz(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::ROTATE_Z << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
}






//U

//WITH DOUBLE AS THETA
const void Circuit::u(unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::U1 << "(" << angle_in_radians << ") qr[" << qubit_index << "];\n";
	}
}

const void Circuit::u(string qreg, unsigned int qubit_index, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::U1 << "(" << angle_in_radians << ") " << qreg << "[" << qubit_index << "];\n";
	}
}

const void Circuit::u(string qreg, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::U1 << "(" << angle_in_radians << ") " << qreg << ";\n";
	}
}




//WITH STRING AS THETA
const void Circuit::u(unsigned int qubit_index, string theta) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::U1 << "(" << theta << ") qr[" << qubit_index << "];\n";
	}
}

const void Circuit::u(string qreg, unsigned int qubit_index, string theta) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::U1 << "(" << theta << ") " << qreg << "[" << qubit_index << "];\n";
	}
}

const void Circuit::u(string qreg, string theta) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::U1 << "(" << theta << ") " << qreg << ";\n";
	}
}





//CU
const void Circuit::cu(unsigned int control, unsigned int target, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << angle_in_radians << ") qr[" << control << "], qr[" << target << "];\n";
	}
}

const void Circuit::cu(string qreg, unsigned int control, unsigned int target, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << angle_in_radians << ") " << qreg << "[" << control << "], " << qreg << "[" << target << "];\n";
	}
}


const void Circuit::cu(string qreg1, unsigned int control, string qreg2, unsigned int target, double angle_in_radians) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << angle_in_radians << ") " << qreg1 << "[" << control << "], " << qreg2 << "[" << target << "];\n";
	}
}



const void Circuit::cu(unsigned int control, unsigned int target, string init, unsigned long long int arg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << init << arg << ") qr[" << control << "], qr[" << target << "];\n";
	}
}

const void Circuit::cu(string qreg, unsigned int control, unsigned int target, string init, unsigned long long int arg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << init << arg << ") " << qreg << "[" << control << "], " << qreg << "[" << target << "];\n";
	}
}


const void Circuit::cu(string qreg1, unsigned int control, string qreg2, unsigned int target, string init, unsigned long long int arg) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << init << arg << ") " << qreg1 << "[" << control << "], " << qreg2 << "[" << target << "];\n";
	}
}




const void Circuit::cu(unsigned int control, unsigned int target, string theta) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << theta << ") qr[" << control << "], qr[" << target << "];\n";
	}
}


const void Circuit::cu(string qreg, unsigned int control, unsigned int target, string theta) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << theta << ") " << qreg << "[" << control << "]," << qreg << "[" << target << "];\n";
	}
}

const void Circuit::cu(string qreg1, unsigned int control, string qreg2, unsigned int target, string theta) {
	if (system == "IBM") {
		outputQASM << fixed << QASM::CU1 << "(" << theta << ") " << qreg1 << "[" << control << "]," << qreg2 << "[" << target << "];\n";
	}
}











//CX
void Circuit::cx(unsigned int control, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_NOT << " qr[" << control << "]" << " , qr[" << target << "];\n";
	}
}


void Circuit::cx(string qreg, unsigned int control, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_NOT << " " << qreg << " [" << control << "]" << " , " << qreg << "[" << target << "];\n";
	}
}

void Circuit::cx(string qreg1, unsigned int control, string qreg2, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_NOT << " " << qreg1 << " [" << control << "]" << " , " << qreg2 << "[" << target << "];\n";
	}
}

void Circuit::cx(string qreg1, string qreg2) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_NOT << " " << qreg1 << "," << qreg2 << ";\n";
	}
}



//CZ
void Circuit::cz(unsigned int control, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_Z << " qr[" << control << "]" << " , qr[" << target << "];\n";
	}
}



void Circuit::cz(string qreg, unsigned int control, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_Z << " " << qreg << "[" << control << "]" << " , " << qreg << "[" << target << "];\n";
	}
}

void Circuit::cz(string qreg1, unsigned int control, string qreg2, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_Z << " " << qreg1 << "[" << control << "]" << " , " << qreg2 << "[" << target << "];\n";
	}
}

void Circuit::cz(string qreg1, string qreg2) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_Z << " " << qreg1 << " , " << qreg2 << ";\n";
	}
}


//CH
void Circuit::ch(unsigned int control, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_HADAMARD << " qr[" << control << "]" << " , qr[" << target << "];\n";
	}
}



void Circuit::ch(string qreg, unsigned int control, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_HADAMARD << " " << qreg << "[" << control << "]" << " , " << qreg << "[" << target << "];\n";
	}
}

void Circuit::ch(string qreg1, unsigned int control, string qreg2, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::CONTROLLED_HADAMARD << " " << qreg1 << "[" << control << "]" << " , " << qreg2 << "[" << target << "];\n";
	}
}



//CCX
void Circuit::ccx(unsigned int control, unsigned int target1, unsigned int target2) {
	if (system == "IBM") {
		outputQASM << QASM::TOFFOLI << " qr[" << control << "]," << " qr[" << target1 << "],"  "qr[" << target2 << "];\n";
	}

}

void Circuit::ccx(string qreg, unsigned int control, unsigned int target1, unsigned int target2) {
	if (system == "IBM") {
		outputQASM << QASM::TOFFOLI << " " << qreg << "[" << control << "]," << qreg << "[" << target1 << "]," << qreg << "[" << target2 << "];\n";
	}

}

void Circuit::ccx(string qreg1, unsigned int control, string qreg2, unsigned int target1, string qreg3, unsigned int target2) {
	if (system == "IBM") {
		outputQASM << QASM::TOFFOLI << " " << qreg1 << "[" << control << "]," << qreg2 << "[" << target1 << "]," << qreg3 << "[" << target2 << "];\n";
	}

}






//CCU1
void Circuit::ccu1(unsigned int control1, unsigned int control2, unsigned int target, string theta) {
	if (system == "IBM") {
		this->cu(control2, target, theta + "/2");
		this->cx(control1, control2);
		this->cu(control2, target, "-" + theta + "/2");
		this->cx(control1, control2);
		this->cu(control1, target, theta + "/2");
	}
}


void Circuit::ccu1(string qreg, unsigned int control1, unsigned int control2, unsigned int target, string theta) {
	if (system == "IBM") {
		this->cu(qreg, control2, target, theta + "/2");
		this->cx(qreg, control1, control2);
		this->cu(qreg, control2, target, "-" + theta + "/2");
		this->cx(qreg, control1, control2);
		this->cu(qreg, control1, target, theta + "/2");
	}
}


void Circuit::ccu1(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, string theta) {
	if (system == "IBM") {
		this->cu(qreg2, control2, qreg3, target, theta + "/2");
		this->cx(qreg1, control1, qreg2, control2);
		this->cu(qreg2, control2, qreg3, target, "-" + theta + "/2");
		this->cx(qreg1, control1, qreg2, control2);
		this->cu(qreg1, control1, qreg3, target, theta + "/2");
	}
}


void Circuit::ccu1(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, double theta) {
	if (system == "IBM") {
		this->cu(qreg2, control2, qreg3, target, theta / 2);
		this->cx(qreg1, control1, qreg2, control2);
		this->cu(qreg2, control2, qreg3, target, -theta / 2);
		this->cx(qreg1, control1, qreg2, control2);
		this->cu(qreg1, control1, qreg3, target, theta / 2);
	}
}


//any arbitrary 3 qubit controlled rotation
void Circuit::ccgate(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, int (*gate)(string qreg_func1, unsigned int qubit1, string qreg_func2, unsigned int qubit2)) {
	if (system == "IBM") {
		(*gate) (qreg2, control2, qreg3, target);
		this->cx(qreg1, control1, qreg2, control2);
		(*gate) (qreg2, control2, qreg3, target);
		this->cx(qreg1, control1, qreg2, control2);
		(*gate) (qreg1, control1, qreg3, target);

	}
}


//for single register only
void Circuit::ccgate(string qreg, unsigned int control1, unsigned int control2, unsigned int target, int (*gate)(string qreg_func, unsigned int qubit1, unsigned int qubit2)) {
	if (system == "IBM") {
		(*gate) (qreg, control2, target);
		this->cx(qreg, control1, control2);
		(*gate) (qreg, control2, target);
		this->cx(qreg, control1, control2);
		(*gate) (qreg, control1, target);

	}
}



//for parameterized gates with multiple quantum registers
void Circuit::ccgate(string qreg1, unsigned int control1, string qreg2, unsigned int control2, string qreg3, unsigned int target, string theta, int (*gate)(string qreg_func1, unsigned int qubit1, string qreg_func2, unsigned int qubit2, string theta)) {
	if (system == "IBM") {
		(*gate) (qreg2, control2, qreg3, target, theta);
		this->cx(qreg1, control1, qreg2, control2);
		(*gate) (qreg2, control2, qreg3, target, theta);
		this->cx(qreg1, control1, qreg2, control2);
		(*gate) (qreg1, control1, qreg3, target, theta);

	}
}



//for parameterized gates with single quantum register
void Circuit::ccgate(string qreg, unsigned int control1, unsigned int control2, unsigned int target, string theta, int (*gate)(string qreg_func, unsigned int qubit1, unsigned int qubit2, string theta_func)) {
	if (system == "IBM") {
		(*gate) (qreg, control2, target, theta);
		this->cx(qreg, control1, control2);
		(*gate) (qreg, control2, target, theta);
		this->cx(qreg, control1, control2);
		(*gate) (qreg, control1, target, theta);
	}
}




//SWAP
void Circuit::swap(unsigned int control, unsigned int target) {
	if (system == "IBM") {
		outputQASM << QASM::SWAP << " qr[" << control << "]" << " , qr[" << target << "];\n";
	}
}


//barrier
const void Circuit::barrier() {



}

const void Circuit::barrier(string qreg) {
	outputQASM << QASM::BARRIER << " " << qreg << ";\n";


}

#endif
