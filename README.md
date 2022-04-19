[![Unitary Fund](https://img.shields.io/badge/Supported%20By-UNITARY%20FUND-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

# A Higher-Level Oracle Description Language (HODL)

A C-Style programming language for Quantum Computers that currently compiles down to OpenQASM 2.0, although efforts are underway to change the target to a quantum intermediate representation (QIR) format.

Language description can be found: https://arxiv.org/abs/2110.12487

Simply download code, navigate to directory, and run make - only for Linux/Unix-based systems (or Cygwin/MinGW) For Windows load src folder into MS Visual Studio and hit build.

Run the compiler with executable name "qc" (with or without prefixing directory depending on whether or not you have added program to path), followed by any compiler options, followed by the file name. This action creates a file named out.qasm (or other, depending on whether it has been specified with "-o") within the directory, which can be run on IBM systems/simulators.

# Ongoing Efforts

It is an ongoing effort to shift the compilation target from OpenQASM 2.0 to a more general Quantum Intermediate Representation (QIR) format, and to build interfaces between current frameworks and HODL. This work has been started and is in its early stages for QISKit:
     
    oracle_instance = init_hodl("""
        super a = 4;
        if(a < 2) {
            mark(a,pi);
        }
    """
	
	circ = oracle_instance.to_circ([inputs_go_here])
	circ.draw(output="mlp")
	

# Notes

Another compiler option supported, "-t", standing for "tape". Prints out internal instruction tape representation of program to view how program is compiled, and which is useful for debugging the program as well as the compiler.

Example: qc data/grover.hodl

Some further dev notes:

- Nested conditionals are buggy as of now
- Classical while loop doesn't work right now
