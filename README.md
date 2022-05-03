[![Unitary Fund](https://img.shields.io/badge/Supported%20By-UNITARY%20FUND-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

# A Higher-Level Oracle Description Language (HODL)

A C-Style programming language for Quantum Computers designed for writing oracles containing arithmetic and logical operations. It currently compiles down to either OpenQASM 2.0 or the Quantum Intermediate Representation (QIR) format, although the latter is in its beta phase. 

Language description can be found: https://arxiv.org/abs/2110.12487

After downloading the source code, navigate to the HODL directory and run "make". This should create an output binary in the bin sub-directory.

To use the compiler, create a new "bin" directory and run make from the root directory. After compilation this should create an executable named "qc", which can be appended to PATH. To run the compiler, specify the --target system (qir/qasm) and the output file (-o).

# Ongoing Efforts

It is an ongoing effort to shift the compilation target from OpenQASM 2.0 to a more general Quantum Intermediate Representation (QIR) format, and to build interfaces between current frameworks and HODL. This work has been started and is in its early stages for QISKit:
     
    oracle_instance = init_hodl("""
        super a = 4;
        if(a < 2) {
            mark(a,pi);
        }
    """)
	
	circ = oracle_instance.to_circ([inputs_go_here])
	circ.draw(output="mlp")
	

# Notes

Another compiler option supported, "-t", standing for "tape". Prints out internal instruction tape representation of program to view how program is compiled, and which is useful for debugging the program as well as the compiler.

Example: qc data/grover.hodl

Some further dev notes:

- Nested conditionals are buggy as of now
- Classical while loop doesn't work right now
