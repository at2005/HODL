#ifndef VALIDATE_H
#define VALIDATE_H


#define ID "IDENTIFIER"
#define NUM "NUMBER"
#define ERROR "ERROR"
#define NONE "NONE"


inline bool is_leaf(Node* node) {
	if(!node) return true;
	if(node->getTToken() == NUM || node->getTToken() == ID) return true;
	return false;
}


/* check if tree is valid
	conditions:
		- If second-last layer is valid
			- Check if second-last layer
			- Verify that return type is valid
*/

// this could be achieved through one-liner but in the future I anticipate 
// more complex scenarios


string get_recursive_type(Node* root) {
	// base case for leaves
	if(is_leaf(root)) return root->getTToken();
	// recusively get the type of 
	string ltype = get_recursive_type(root->getLeftChild());
	string rtype = get_recursive_type(root->getRightChild());
	if(root->getTToken() == "OPERATOR" && root->getTValue().size() > 1 || root->getTValue() == "=") return NONE;
	if((ltype == ID || rtype == ID) && (ltype != NONE && rtype != NONE)) return ID;
	return NONE;
}




bool validate_tree(Node* root) {
	string ltype = get_recursive_type(root->getLeftChild());
	string rtype = get_recursive_type(root->getRightChild());
	if(ltype == NONE or rtype == NONE) return false;
	return true;

}


#endif
