# Q
The Q Programming Language

A C-Style programming language for Quantum Computers that compiles down to OpenQASM. 

Description/Paper on ArXiv soon...

Simply download code and run make, only for Linux/Unix-based systems (or Cygwin/MinGW)
For Windows load src folder into MS Visual Studio and hit build.

Some notes:
  - Interpreter for classical functions disabled. Check resource estimator since that is where classical instructions are performed. This means no loops/classical stuffs. Enabling gives segmentation fault on Linux systems. Works fine on Windows (32-bit) version. If you're using Windows you can uncomment the line  Strange.
  - memory allocated with new is not always freed. I've tried my best to hunt down as many memory leaks as possible, but some may still remain due to sheer laziness while programming.
  - Bugs exist.
  - I know some parts don't make sense, and to be honest, I've tried to figure out what they do too, but removing them gives a ton of errors/undefined compiler output.
  - Nested conditionals not supported yet.



