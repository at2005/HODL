[![Unitary Fund](https://img.shields.io/badge/Supported%20By-UNITARY%20FUND-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

# A Higher-Level Oracle Description Language (HODL)

A C-Style programming language for Quantum Computers designed for writing oracles containing arithmetic and logical operations. It currently compiles down to either OpenQASM 2.0 or the Quantum Intermediate Representation (QIR) format. 

Language description can be found: https://arxiv.org/abs/2110.12487

To use the compiler, create a new "bin" directory from the root directory and run make. After compilation this should create an executable named "hodl", which can be appended to PATH. To run the compiler, specify the --target system (qir/qasm) and the output file (-o).

# QISKit integration

One can also use HODL within QISKit, provided that the executable is in PATH and the necessary library (in "modules" sub-directory) has been imported: 

     
    oracle_instance = compile_oracle("""
	function foo(a) {
        	if(a < 2) {
            		mark(a,pi);
        	}
	}
    """)
	
	qr = QuantumRegister(3)
	circ = oracle_instance.to_circ([qr])
	circ.draw(output="mlp")
	



Some further dev notes:

- Nested conditionals give segmentation faults
- Classical while loop always runs to infinity
