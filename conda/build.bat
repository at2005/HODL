:: compile and test run the HODL project
mkdir bin
g++ src/circuit/circuit.cpp src/compiler/source.cpp -std=c++11 -o bin/hodl.exe
:: execute bin/hodl.exe with the test file "data/grover.hodl" and -o to output the result to "out.qasm"
cd bin
hodl.exe
