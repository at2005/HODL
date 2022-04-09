
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class QIRCirc {
	private:
		ofstream out;
	public:
		QIRCirc() {
			out.open("out.qir");
		}
		
		~QIRCirc() {
			out.close();
		}		

		// llvm create functions
		void llvm_fstart(string fname) {
			out << "define @" << fname << " { \n";	
		}

		void llvm_fterm() {
			out << "}\n";
		}

};


int main() {
	QIRCirc circ;
	circ.llvm_fstart("o1");
	circ.llvm_fterm();


}
