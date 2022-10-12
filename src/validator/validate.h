#include "../parser/ast.h"

inline bool is_leaf(Node* node) {
	if(!node) return true;
	if(node->getTToken() == "NUMBER" || node->getTToken() == "IDENTIFIER") return true;
	return false;
}


/* check if tree is valid
	conditions:
		- If second-last layer is valid
			- Check if second-last layer
			- Verify that return type is valid
*/
bool validate_tree(Node* root) {
	if(!is_leaf(root)) {
		if(is_leaf(root->getLeftChild()) && is_leaf(root->getRightChild())) {
				

		}	


	}


}


