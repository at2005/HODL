# Q
The Q Programming Language

A C-Style programming language for Quantum Computers that compiles down to OpenQASM. 

Language description/guide coming soon...

Simply download code, navigate to directory, and run make - only for Linux/Unix-based systems (or Cygwin/MinGW)
For Windows load src folder into MS Visual Studio and hit build.

Run the compiler with executable name "qc" (with or without prefixing directory depending on whether or not you have added program to path). This action creates a file named program.qasm within the directory, which can in turn be run on IBM systems/simulators.

One compiler option supported, "-t", standing for "tape". Prints out internal instruction tape representation of program (required for unitarity/reversible functions etc) which is useful for debugging the compiler.

Example: qc data/program.q

Some notes:
  - Interpreter for classical functions disabled for Linux. Check resource estimator since that is where classical instructions are expanded and evaluated (think of it like macro-expansion time in LISP). Anyways, this means loops/classical computation don't work, since enabling gives a prompt segfault on Linux systems. Works fine on Windows (32-bit) version. If you're using Windows you can uncomment line 44 in Compiler/Resources.h.
  - Bugs exist.
  - I know some parts don't make sense, and to be honest, I've tried to figure out what they do too, but removing them gives a ton of errors/undefined compiler output.
  - Nested conditionals still in progress. Normal if-else statements work fine.
