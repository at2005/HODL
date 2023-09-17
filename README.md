[![Unitary Fund](https://img.shields.io/badge/Supported%20By-UNITARY%20FUND-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

# A Higher-Level Oracle Description Language (HODL)

A C-Style programming language for Quantum Computers designed for writing oracles containing arithmetic and logical operations. It currently compiles down to either OpenQASM 2.0 or the Quantum Intermediate Representation (QIR) format. 

Language description can be found: https://arxiv.org/abs/2110.12487

# Usage

```
hodl --target qasm/qir -o your_output_file your_source_file.hodl
```

# Installation

Pre-compiled executables are included in the latest Release.

# Installation from Source

Run the following commands in the terminal:

1. Clone repository

```
$ git clone https://github.com/at2005/HODL
```

2. Navigate to the HODL directory

```
$ cd HODL
```

3. Create an empty "bin" directory

```
$ mkdir bin
```

4. Run the makefile

```
$ make
```

If you would like not to constantly have to refer back to the absolute path of the program, then add the "hodl" executable in the "bin" directory to the $PATH environment variable.


# QISKit integration

HODL can be used within QISKit, provided that the executable is in $PATH and the HODL Python library has been installed and imported: 

```$ conda install -c at2005 hodl_qiskit```

```     
from qiskit import *
from src.hodl_qiskit import *


oracle_instance = init_oracle("""
   function hodl_oracle(super a) {
       if(a > 5) {
           mark(a,pi);
       }
   }
""")


qr = QuantumRegister(name="reg0", size=3)
circ = oracle_instance.to_circ([qr])
```	





Some further dev notes:

- Nested conditionals give segmentation faults
- Classical while loop always runs to infinity
