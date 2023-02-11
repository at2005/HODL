:: compile and test run the HODL project
mkdir bin
g++ src/circuit/circuit.cpp src/compiler/source.cpp -std=c++11 -o bin/hodl.exe
bin/hodl.exe --target qasm -o out.qasm data/grover.hodl
