

qc: src/circuit/circuit.cpp src/compiler/source.cpp
	g++ src/circuit/circuit.cpp src/compiler/source.cpp -std=c++11 -o bin/qc
clean:
	cd src/circuit
	rm -rf *.o
	cd ~/Q/src/compiler
	rm -rf *.o
	cd ~/Q
	rm qc
	
.PHONY: all clean


