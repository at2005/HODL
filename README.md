[![Unitary Fund](https://img.shields.io/badge/Supported%20By-UNITARY%20FUND-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

# A Higher-Level Oracle Description Language (HODL)

A C-Style programming language for Quantum Computers designed for writing oracles containing arithmetic and logical operations. It currently compiles down to either OpenQASM 2.0 or the Quantum Intermediate Representation (QIR) format, although the latter's development is still in its beta phase. 

Language description can be found: https://arxiv.org/abs/2110.12487

To use the compiler, create a new "bin" directory from the root directory and run make. After compilation this should create an executable named "qc", which can be appended to PATH. To run the compiler, specify the --target system (qir/qasm) and the output file (-o).

# QISKit integration

One can also use HODL within QISKit, provided that the executable is in PATH and the necessary library (in "modules" sub-directory) has been imported: 

     
    oracle_instance = init_hodl("""
	function foo(a) {
        	if(a < 2) {
            		mark(a,pi);
        	}
	}
    """)
	
	qr = QuantumRegister(3)
	circ = oracle_instance.to_circ([qr])
	circ.draw(output="mlp")
	

# Notes

Another compiler option supported, "-t", standing for "tape". Prints out internal instruction tape representation of program to view how program is compiled, and which is useful for debugging the program as well as the compiler.

Example: qc data/grover.hodl

Some further dev notes:

- Nested conditionals are buggy as of now
- Classical while loop doesn't work right now
