from qiskit import *
from qiskit.visualization import plot_histogram

import re
import subprocess
import os

### define a class to hold hodl oracle code alongside parameters
### this allows many instances of an oracle to be defined --> not limited to
### input-dependent compilation
class HODLOracle:
    def __init__(self, func_name, init_params, program):
        ### accepts body of the oracle, parameters to the oracle in order, and the oracle name
        self.program = program
        self.params = init_params
        self.fname = func_name
        
    ### compilation function --> this takes an oracle instance, generates a HODL program,
    ### invokes the compiler binary, converts the output assembly to a qiskit circuit, and returns it
    def to_circ(self,params):
        main = "function main() {"
        ### iterate over each parameter and if a quantum register then declare it as
        ### a |0>^n register. This initialization is such so that the circuit can be concatenated
        ## with any desired input register
        param_lst = []
        for q in params:
            if isinstance(q, QuantumRegister):
                main += f"\nsuper {q.name} = {2**(q.size)};\nH({q.name});"
                param_lst.append(q.name)
            else:
                param_lst.append(str(q))
            
        
        main += f"\n{self.fname}({','.join(param_lst)});"
        result = self.program + main + "\n}"
        f = open("program.hodl", "w")
        f.write(result)
        f.close()
        os.system("qc --target qasm -o out.qasm program.hodl")
        circ = QuantumCircuit.from_qasm_file("out.qasm")
        os.remove("program.hodl")
        os.remove("out.qasm")
        return circ
        
### takes an oracle and outputs a HODL object      
def compile_oracle(code):
    fname = re.search("function (.*)\(", code).group(1)
    params = re.search("\((.*)\)",code).group(1)
    params = params.split(",")
    ### parameter map --> {type: name}
    param_dict = {}
    for p in params:
        p = p.split()
        param_dict[p[0]] = p[1]
    
    oracle = HODLOracle(fname, param_dict, code)
    return oracle

    
