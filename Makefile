all: qc
	g++ src/circuit/Circuit.o src/compiler/Source.o -o qc

CC = $(CXX)

qc: src/circuit/Circuit.o src/compiler/Source.o 


clean:
	cd src/circuit
	rm -rf *.o
	cd ~/Q/src/compiler
	rm -rf *.o
	cd ~/Q
	rm qc
	
.PHONY: all clean


