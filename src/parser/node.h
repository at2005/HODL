#ifndef NODE_H
#define NODE_H

#include "../lexer/dictionary.h"


using namespace std;



class Node {
public:
	Node() {
		this->Purpose = "OPERATOR_NDEF";
		this->Parent = nullptr;
		this->result_register = "";
		this->func_call_params = {};
		this->LeftChild = nullptr;
		this->RightChild = nullptr;
		
	}

	Node(Pair input) {
		this->Purpose = "OPERATOR_NDEF";
		this->Parent = nullptr;
		this->TokenValue = input;
		this->result_register = "";
		this->func_call_params = {};
		this->LeftChild = nullptr;
		this->RightChild = nullptr;
	}

	//Constructor number 1 - First parental node
	Node(Pair input, string purpose) {
		//sets token
		this->TokenValue.setToken(input.getToken());
		//sets value
		this->TokenValue.setValue(input.getValue());
		this->Parent = NULL;
		this->Purpose = purpose;
		this->result_register = "";
		this->func_call_params = {};
		this->LeftChild = nullptr;
		this->RightChild = nullptr;

	}




	//Constructor 2 - Child nodes
	Node(Node* ParentAdd, string purpose_of_child_node, Pair input) {
		//sets token
		this->TokenValue.setToken(input.getToken());
		//sets value
		this->TokenValue.setValue(input.getValue());
		this->Parent = ParentAdd;
		this->Purpose = purpose_of_child_node;
		this->result_register = "";
		this->func_call_params = {};
		this->LeftChild = nullptr;
		this->RightChild = nullptr;

	}



	//Function to create right child nodes

	void createRightChild(Pair newChild, string childType) {
		//if (this->RightChild == NULL) {
			this->RightChild = new Node(this, childType, newChild);
		//}
	}

	void createRightChild(Node rchild) {
		//if (this->RightChild == NULL) {
			this->RightChild = &rchild;
			rchild.setParent(this);
		//}
	}

	void createRightChild(Node* rchild) {
		//if (this->RightChild == NULL) {
			this->RightChild = rchild;
			rchild->setParent(this);
		//}
	}

	//deletes right child
	void deleteRightChild() {
		this->RightChild = NULL;
	}

	bool hasRightChild() {
		if (this->RightChild != NULL) {
			return true;
		}

		else {
			return false;
		}
	}

	Node* getRightChild() {
		//if (this->RightChild != (Node*)NULL) {
			return this->RightChild;
		//}
	}

	void setRightChild(Node* rchild) {
		this->RightChild = rchild;
	}


	//functions to create left child nodes
	void createLeftChild(Pair newChild, string childType) {
		//if (this->LeftChild == NULL) {
			this->LeftChild = new Node(this, childType, newChild);
			has_left_child = true;
		//}

	}


	void createLeftChild(Node lchild) {
		//if (this->LeftChild == NULL) {
			this->LeftChild = &lchild;
			lchild.setParent(this);
			has_left_child = true;
		//}
	}


	void createLeftChild(Node* lchild) {
		//if (this->LeftChild == NULL) {
			this->LeftChild = lchild;
			lchild->setParent(this);
			has_left_child = true;
		//}
	}

	void createLeftChild() {
		//if (this->LeftChild == NULL) {
			this->LeftChild = new Node();
			this->LeftChild->setParent(this);
			has_left_child = true;
		//}
	}


	//deletes left child
	void deleteLeftChild() {
		this->LeftChild = NULL;

	}

	bool hasLeftChild() {
		if (this->has_left_child) {
			return true;
		}

		else {
			return false;
		}
	}

	Node* getLeftChild() {
		//if (this->LeftChild != (Node*)NULL) {
			return this->LeftChild;
		//}

	}



	void setLeftChild(Node* lchild) {
		this->LeftChild = lchild;
	}





	//gets parental memory address
	Node* getParent() {
		if (!(this == nullptr)) {
			return Parent;
		}
	}

	//changes the parental memory address
	void setParent(Node* new_parent) {
		this->Parent = new_parent;
	}


	//sets the parental memory address to "NULL"
	void deleteParent() {
		this->Parent = NULL;
	}

	bool hasParent() {
		if (this->Parent != NULL) {
			return true;
		}

		else {
			return false;
		}
	}


	//returns token and value
	const string getTToken() {
		return TokenValue.getToken();
	}

	const string getTValue() {
		return TokenValue.getValue();
	}

	//gets the purpose of the Node
	const string getPurpose() {
		return Purpose;
	}

	//sets the purpose of the node
	void setPurpose(string new_purpose) {
		this->Purpose = new_purpose;
	}

	//sets the "Token" component of "TokenValue"
	void setTreeToken(string newToken) {
		this->TokenValue.setToken(newToken);
	}

	//sets the "Value" component of "TokenValue"
	void setTreeValue(string newValue) {
		this->TokenValue.setValue(newValue);
	}

	//sets the TokenValue variable of type "Pair"
	void setTreeTV(Pair newTV) {
		this->TokenValue = newTV;
	}

	Pair getTreeTV() {
		return this->TokenValue;
	}



	bool is_classical() {
		if (this) {
			return this->is_classical_var;
		}

		return false;
	}

	void set_classical() {
		is_classical_var = true;
	}

	void set_classical(Node* Parent) {
		if (Parent->is_classical()) {
			this->set_classical();
		}
	}

	void flip_classical() {
		is_classical_var = false;
	}

	void set_result_register(string result) {
		this->result_register = result;
	}


	string get_result_register() {
		return this->result_register;
	}

	void push_func_param(Pair pr) {
		this->func_call_params.push_back(pr);
	}

	vector<Pair> get_func_params() {
		return this->func_call_params;
	}

	


private:
	//token, value pair
	Pair TokenValue;
	Node* LeftChild;
	Node* RightChild;
	vector<Pair> func_call_params;
	

	Node* Parent = NULL;
	bool is_classical_var = false;

	string result_register;

	//purpose string holds the purpose of the node, e.g. assigment
	string Purpose;

	bool has_left_child;
	bool has_right_child;
};

#endif
