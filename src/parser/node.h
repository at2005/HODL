#ifndef NODE_H
#define NODE_H

#include "../lexer/dictionary.h"


using namespace std;



// Node class for AST
class Node {
public:
	// first constructor for blank init
	Node() {
		this->Purpose = "OPERATOR_NDEF";
		this->Parent = nullptr;
		this->result_register = "";
		this->func_call_params = {};
		this->LeftChild = nullptr;
		this->RightChild = nullptr;
		
	}


	// for init with token-value
	Node(Pair input) {

		this->Purpose = "OPERATOR_NDEF";
		this->Parent = nullptr;
		this->TokenValue = input;
		this->result_register = "";
		this->func_call_params = {};
		this->LeftChild = nullptr;
		this->RightChild = nullptr;
	}


	// with a purpose too	
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
	Node(Node* ParentAdd, string child_purpose, Pair input) {
		//sets token
		this->TokenValue.setToken(input.getToken());
		//sets value
		this->TokenValue.setValue(input.getValue());
		this->Parent = ParentAdd;
		this->Purpose = child_purpose;
		this->result_register = "";
		this->func_call_params = {};
		this->LeftChild = nullptr;
		this->RightChild = nullptr;

	}


	


	//Function to create right child nodes

	void createRightChild(Pair newChild, string childType) {
		this->RightChild = new Node(this, childType, newChild);
	}

	void createRightChild(Node rchild) {
		this->RightChild = &rchild;
		rchild.setParent(this);
	}

	void createRightChild(Node* rchild) {
			this->RightChild = rchild;
			rchild->setParent(this);
	}

	//deletes right child
	void deleteRightChild() {
		delete (this->RightChild);
		this->RightChild = NULL;
	}


	Node* getRightChild() {
			return this->RightChild;
	}

	void setRightChild(Node* rchild) {
		this->RightChild = rchild;
	}


	//functions to create left child nodes
	void createLeftChild(Pair newChild, string childType) {
		this->LeftChild = new Node(this, childType, newChild);
		has_left_child = true;

	}


	void createLeftChild(Node lchild) {
			this->LeftChild = &lchild;
			lchild.setParent(this);
			has_left_child = true;
	}


	void createLeftChild(Node* lchild) {
			this->LeftChild = lchild;
			lchild->setParent(this);
			has_left_child = true;
	}

	void createLeftChild() {
		this->LeftChild = new Node();
		this->LeftChild->setParent(this);
		has_left_child = true;
	}


	//deletes left child
	void deleteLeftChild() {
		delete (this->LeftChild);
		this->LeftChild = NULL;

	}

	Node* getLeftChild() {
		return this->LeftChild;

	}



	void setLeftChild(Node* lchild) {
		this->LeftChild = lchild;
	}





	//gets parental memory address
	Node* getParent() {
		return Parent;
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
		if (this->Parent) return true;
		return false;


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
		return this->is_classical_var;

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

	bool hasRightChild() {
		return RightChild;
	}
	

	bool hasLeftChild() {
		return LeftChild;
	}

	void alloc_self() {
		this->is_alloc = true;
	}

	bool isalloc() {
		return this->is_alloc;

	}


	void* operator new(size_t size) {
		void* mem = ::operator new(size);
		Node* new_node  = (Node*)mem;
		new_node->alloc_self();
		return mem;

	}


	void operator delete(void* mem) {
		Node* new_node = (Node*)mem;
		if(new_node->isalloc()) free(mem); 

	}

private:
	//token, value pair
	Pair TokenValue;
	Node* LeftChild;
	Node* RightChild;
	vector<Pair> func_call_params;
	

	Node* Parent = NULL;
	
	string result_register;

	//purpose string holds the purpose of the node, e.g. assigment
	string Purpose;
	bool is_alloc = false;
	bool is_classical_var = false;
	bool has_left_child;
	bool has_right_child;
};

#endif
