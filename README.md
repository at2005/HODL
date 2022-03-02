# A Higher-Level Oracle Description Language (HODL)

A C-Style programming language for Quantum Computers that currently compiles down to OpenQASM 2.0, although efforts are underway to change the target to a quantum intermediate representation (QIR) format.

Language description can be found: https://arxiv.org/abs/2110.12487

Simply download code, navigate to directory, and run make - only for Linux/Unix-based systems (or Cygwin/MinGW) For Windows load src folder into MS Visual Studio and hit build.

Run the compiler with executable name "qc" (with or without prefixing directory depending on whether or not you have added program to path), followed by any compiler options, followed by the file name. This action creates a file named out.qasm (or other, depending on whether it has been specified with "-o") within the directory, which can be run on IBM systems/simulators.

It is also an ongoing effort to create bridges between current frameworks and HODL such that oracles can be written in those frameworks with the support of the HODL compiler. This would eliminate the need for having to write, compile & load a separate HODL program, and would integrate the language into current workflows. For example, a QISKit program incorporating HODL may look like:
    
    from qiskit.HODL import hodl2circ
    circ = hodl2circ("""
        super a = 4;
        if(a < 2) {
            mark(a,pi);
        }
    """
    circ.measure_all();

# Notes

Another compiler option supported, "-t", standing for "tape". Prints out internal instruction tape representation of program to view how program is compiled, and which is useful for debugging the program as well as the compiler.

Example: qc data/grover.hodl

Some further dev notes:

- Nested conditionals are buggy as of now
- Classical while loop doesn't work right now
