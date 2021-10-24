# HODL program illustrating Grover's algorithm.

# oracle takes a superposition as an input
oracle some_oracle(super var) { 
	# check if the input variable multiplied by four is less than four
	if(var * 4 < 4) {
      # apply a phase of pi to var if var *  4 < 4
		mark(var,pi); 
	}
}


function main() {
	#declare a uniform superposition of 3 qubits
	super variable = 8;
	# "filter" function is diffusion operator
	filter(some_oracle(variable));
	measure variable;
}
