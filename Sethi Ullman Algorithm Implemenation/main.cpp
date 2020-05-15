#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct Node {
	char val=' ';
	int label=0;
	Node *leftNode=NULL;
	Node *rightNode=NULL;
};

string input() {
	string input;
	cout << "Input: ";
	cin >> input;
	bool check=true;
	for(int i=0; i<input.length(); i++) {
		if(!((int(input[i])>=48 && int(input[i])<=57) || input[i]=='+' || input[i]=='-' || input[i]=='(' || input[i]==')' || input[i]=='/' || input[i]=='*')) {
			check=false;
			break;
		}
	}
	if(input.length()==0 || !check) {
		cout << "\nInvalid Input\n";
		exit(0);
	}
	return input;
}

stack<Node*> local;
stack<Node*> global;

void reduce(bool Global, bool Local) {
	if(Global) {
		if(global.size()==3) {
			//cout << "\nReduce Global Entry\n";
			Node *right=(global.top());
			//cout << "Right: " << (right)->val << "\n";
			global.pop();
			Node *root=(global.top());
			//cout << "Root: " << (root)->val << "\n";
			global.pop();
			Node *left=(global.top());
			//cout << "left: " << (left)->val << "\n";
			global.pop();
			(root)->leftNode=(left);
			(root)->rightNode=(right);
			global.push(root);
		}
	}
	if(Local) {
		if(local.size()==3) {
			//cout << "\nReduce Local Entry\n";
			Node *right=(local.top());
			//cout << "Right: " << (right)->val << "\n";
			local.pop();
			Node *root=(local.top());
			//cout << "Root: " << (root)->val << "\n";
			local.pop();
			Node *left=(local.top());
			//cout << "left: " << (left)->val << "\n";
			local.pop();
			(root)->leftNode=(left);
			(root)->rightNode=(right);
			global.push(root);
		}
		else if(local.size()==1) {
			global.push(local.top());
			local.pop();
		}
	}
	/*cout << "\nReduce Local Exit\n";
	cout << "\nReduce: local stack size: " << local.size() << "\n";
	cout << "\nReduce: Global stack size: " << global.size() << "\n";
	if(local.size()!=0){
		cout << "Reduce: Local Stack Top: " << (local.top())->val << "\n";
	}
	if(global.size()!=0){
		cout << "Reduce: Global Stack Top: " << (global.top())->val <<"\n";
	}*/
}



void printTree(Node *root) {
	if(root==NULL) {
		return;
	}
	if(root->leftNode) {
		printTree(root->leftNode);
	}
	cout << "Val: '" << root->val << "' Label: " << root->label << "    ";
	if(root->rightNode) {
		printTree(root->rightNode);
	}
}

void label(Node *root, bool left) {
	if(root==NULL) {
		return;
	}
	if(root->leftNode) {
		label(root->leftNode, true);
	}
	if(root->rightNode) {
		label(root->rightNode, false);
	}
	if(!root->leftNode && !root->rightNode) {
		if(left) {
			root->label=1;
		}
		else {
			root->label=0;
		}
	}
	else {
		int left=root->leftNode->label;
		int right=root->rightNode->label;
		if(left==right) {
			root->label=left+1;
		}
		else {
			root->label=max(left,right);
		}
	}
}

int NoReg=2, NoTemp=100;
int noRegUsed=0;
int NoTempUsed=0;
void codeGen(Node *root, bool left) {
	if(root==NULL) {
		cout << "\nCheck 1 \n";
		return;
	}
	//case 1
	if(!root->leftNode && !root->rightNode) {
		if(left) {
//cout << "\ncase 1\n";
			noRegUsed++;
			cout << "MOV " << root->val << " , R" << noRegUsed << "\n"; 
		}
	}
	//case 2
	else if(root && root->rightNode->label==0) {
//cout << "\ncase 2\n";
		codeGen(root->leftNode, true);
		cout << "OP" << root->val << " " << root->rightNode->val  << " , R" << noRegUsed << "\n";	
	}
	//case 3
	else if((1<=root->leftNode->label) && (root->leftNode->label < root->rightNode->label)
			&& (root->label <= (NoReg-noRegUsed))) {
//cout << "\ncase 3\n";
		codeGen(root->rightNode, false);
		//noRegUsed++;
		codeGen(root->leftNode, true);
		noRegUsed++;
		cout << "OP" << root->val << " R" << noRegUsed-1 << " , R" << noRegUsed << "\n"; 
		noRegUsed--;
	}
	else if((1<=root->rightNode->label) && (root->rightNode->label<=root->leftNode->label) 
		&& (root->label <= (NoReg-noRegUsed))) {
//cout << "\ncase 4\n";
		codeGen(root->leftNode, true);
		//noRegUsed++;
		codeGen(root->rightNode, true);
		cout << "OP" << root->val << " R" << noRegUsed << " , R" << noRegUsed -1 << "\n"; 
		noRegUsed--; 
	}
	else if((root->leftNode->label > (NoReg-noRegUsed)) && (root->rightNode->label > (NoReg-noRegUsed))) {
//cout << "\ncase 5\n";
		NoTempUsed++;
		codeGen(root->rightNode, false);
		cout << "MOV R" << noRegUsed << " , T" << NoTempUsed << "\n";
		codeGen(root->leftNode, true);
		cout << "MOV T" << NoTempUsed << " , R" << noRegUsed << "\n";
		NoTempUsed--;
	}
	else {
		cout << "\nError\n";
	}
}

int main() {
	Node root;
	string in=input();
	stack<char> paren;
	int pos=0;
	do {
		/*cout << "\n====Test====\nin[" << pos << "] = " << in[pos] << "\n";
		if(local.size()!=0){
			cout << "Local Stack Top: " << (local.top())->val << "\n";
		}
		if(global.size()!=0){
			cout << "Global Stack Top: " << (global.top())->val <<"\n";
		}*/
		if(global.size()==3) {
			reduce(true,false);
		}
		if(in[pos]=='(') {
			//cout << "\nParen Push: " << in[pos] << "\n";  
			paren.push(in[pos++]);
		}
		else if(in[pos]!=')') {
			Node *node=new Node;
			node->val=in[pos++];
			local.push(node);			
		}
		else if(in[pos]==')') {
			//cout << "\nParen POP: " << paren.top() << "\n";  
			paren.pop();
			pos++;
			reduce(false,true);
			Node *node=new Node;
			if(in[pos]=='\0') {
				break;
			} 
			if(in[pos]==')') {
				continue;
			} 
			node->val=in[pos++];
			global.push(node);
		}
	} while(!paren.empty() && pos<in.size());
	root=*global.top();
	/*if(local.size()!=0){
		cout << "While Exit: Local Stack Top: " << (local.top())->val << "\n";
	}
	if(global.size()!=0){
		cout << "While Exit: Global Stack Top: " << (global.top())->val <<"\n";
	}*/
	cout << "\nExpression Tree Made\n";
	cout << "\nRoot Top: " << root.val << "\n";
	label(&root,true);
	cout << "\nSethi Ullman: Labeling Done\n\n";
	printTree(&root);
	cout << "\n\nSethi Ullman: CodeGen\n\n";
	cout << "Number of Available Registers: ";
	cin >> NoReg;
	cout << "\n\n";
	if(NoReg<2) {
		cout << "Sorry Number of Registers can't be less than 2; Default Value of 2 used\n\n";
		NoReg=2;
	}
	codeGen(&root,true);
	cout << "\n\n======End======\n\nBy: Ravi Jain (411764) (CSE)\n\n";
	return 0;
}