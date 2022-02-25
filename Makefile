

qc: src/circuit/Circuit.cpp src/compiler/Source.cpp
	g++ src/circuit/Circuit.cpp src/compiler/Source.cpp -std=c++11 -o bin/qc
clean:
	cd src/circuit
	rm -rf *.o
	cd ~/Q/src/compiler
	rm -rf *.o
	cd ~/Q
	rm qc
	
.PHONY: all clean


