from qiskit import *
import re
import subprocess
import os

""" define a class to hold hodl oracle code alongside parameters
this allows many instances of an oracle to be defined --> not limited to
input-dependent compilation 
"""
class HODLOracle:
    def __init__(self, func_name, init_params, program):
        ### accepts body of the oracle, parameters to the oracle in order, and the oracle name
        self.program = program
        self.params = init_params
        self.fname = func_name
        
    """compilation function --> this takes an oracle instance, generates a HODL program,
    invokes the compiler binary, converts the output assembly to a qiskit circuit, and returns it
    """
    def to_circ(self,params):
        main = "function main() {"
        """iterate over each parameter and if a quantum register then declare it as
         a |0>^n register. This initialization is such so that the circuit can be concatenated
         with any desired input register
         """
    
        # classical index serves as counter for classically passed values, ie phase angles
        cindex = 0
        for q in params:
            if isinstance(q, QuantumRegister):
                main += f"\nsuper {q.name} = {2**(q.size)};\nH({q.name});"
            else:
                main += f"\nint c{cindex} = {q.name};"
                cindex += 1
        
        # call function
        main += f"\n{self.fname}({','.join([p.name for p in params])});"
        result = self.program + main + "\n}"
        f = open("program.hodl", "w")
        f.write(result)
        f.close()
        subprocess.call(["./qc", "program.hodl"])
        circ = QuantumCircuit.from_qasm_file("out.qasm")
        os.remove("program.hodl")
        os.remove("out.qasm")
        return circ
        
### takes an oracle and outputs a HODL object      
def init_hodl(code):
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
    

