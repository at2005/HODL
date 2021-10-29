# The HODL Programming Language

A C-Style programming language for Quantum Computers that compiles down to OpenQASM.

Language description can be found: https://arxiv.org/abs/2110.12487

Simply download code, navigate to directory, and run make - only for Linux/Unix-based systems (or Cygwin/MinGW) For Windows load src folder into MS Visual Studio and hit build.

Run the compiler with executable name "qc" (with or without prefixing directory depending on whether or not you have added program to path). This action creates a file named program.qasm within the directory, which can be run on IBM systems/simulators.

One compiler option supported, "-t", standing for "tape". Prints out internal instruction tape representation of program to view how program is compiled, and which is useful for debugging the program as well as the compiler.

Example: qc data/program.q

Some further dev notes:

- Nested conditionals are buggy as of now
- Classical while loop doesn't work right now
