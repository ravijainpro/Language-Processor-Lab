#include "lex.h"
#include <stdexcept>
#define LENGTH 100

struct Node {
	string data;
	char op;
};

int tree_lvl=0;

void tree_lvl_print() {
	for(int i=0; i<tree_lvl; i++) {
		cout << "---";
	}
}

void LookAhead();
char look_up(int);
Node ID();
Node RetainParantheses(Node);
Node paren();
Node factor();
Node term();
Node express();
string start_line();

int tokens[LENGTH];
int no_token;
int lookahead;
int curr_position=-1; 

void init_tokens() {
	cout << "\ninit_token\n";
	for(int i=0; i<LENGTH; i++) {
		tokens[i]=-1;
	}
}

void LookAhead() {
	cout << "LookAhead\n";	
	if(curr_position > no_token) {
		cout << "END OF INPUT IS NOT ACCEPTED\n";
		throw std::invalid_argument( "ERROR 1" ); 
	}
	do{
		++curr_position;
		if(tokens[curr_position]==-1) {
			break;
		}
	}
	while(tokens[curr_position]==-1);
	cout << "curr_position: " <<curr_position <<"\n";
}

char look_up(int x) {
	cout << "look_up\n";
	char temp;
	switch(x) {
		case 65:temp='A'; 
				break;
		case 66:temp='B';
				break;
		case 67:temp='C';
				break;
		case 68:temp='D';
				break;
		case 69:temp='E';
				break;
		case 70:temp='F';
				break;
		case 71:temp='G';
				break;
		case 72:temp='H';
				break;
		case 73:temp='I';
				break;
		case 10:temp='+';
				break;
		case 11:temp='-';
				break;
		case 21:temp='*';
				break;
		case 22:temp='/';
				break;
		case 31:temp='(';
				break;
		case 32:temp=')';
				break; 
		default: temp=' ';
	}
	return temp;
}

string start_line() {
	cout << "start_line\n";
	LookAhead();
	Node var = express();
	if(curr_position>no_token) {
		cout << "RETURN\n";
		tree_lvl_print();
		tree_lvl--;
		return var.data;
	}
	if(tokens[curr_position]==-1) {
		cout << "Found unexpected character '" << look_up(tokens[curr_position]) << "' at position " << curr_position << "\n";
		throw std::invalid_argument( "ERROR 4" ); 	
	}
	tree_lvl_print();
	tree_lvl--;
	cout << "return start_line\n";
	return var.data;
}

Node express() {
	tree_lvl++;
	tree_lvl_print();	
	cout << "express\n";
	Node Lside = term();
	int op=tokens[curr_position];
	if (op != 10 && op != 11) {
		tree_lvl_print();
		tree_lvl--;
		cout << "express Lside Return\n";	
	    return Lside;
	}
	cout << "+|- Encountered\n";
    LookAhead();
	Node Rside = term();
/*    if(op == 10 && (Rside.op == '-' || Rside.op == '+')) {
       // Rside = RetainParantheses(Rside);
    } */
	Node to_return;
    to_return.data=Lside.data + " " + look_up(op) + " " + Rside.data;
    to_return.op = look_up(op);
	tree_lvl_print();
	tree_lvl--;
    cout << "express Return\n";	
	return to_return;    
}

Node term() {
	tree_lvl++;
	tree_lvl_print();
	cout << "term\n";
	Node Lside = factor();
	if(curr_position>no_token) {
		tree_lvl_print();
		tree_lvl--;
		cout << "\nRETURN\n";	
	    return Lside;
	}
	int op=tokens[curr_position];
	if(op!=21 && op!=22) {
		tree_lvl_print();
		tree_lvl--;
		cout << "term Lside Return\n";	
	    return Lside;
	}
	cout << "*|/ Encountered\n";
	LookAhead();
	Node Rside = factor();
	if(Lside.op == '+' || Lside.op == '-') {
		Lside = RetainParantheses(Lside);
	}
	if(Rside.op == '+' || Rside.op == '-' || (op == 21 && (Rside.op == '/' || Rside.op == '*'))) {
            Rside = RetainParantheses(Rside);
    }
    Node to_return;
    to_return.data = Lside.data + " " + look_up(op) + " " + Rside.data;
    to_return.op = look_up(op);\
	tree_lvl_print();
	tree_lvl--;
    cout << "term Return\n";	
	return to_return;
}

Node factor() {
	tree_lvl++;
	tree_lvl_print();
	cout << "factor\n";	
	Node var;
	int curr_token=tokens[curr_position];
	if(curr_token==31) {
		var = paren();
	} else if(curr_token!=31 && curr_token!=32 && curr_token!=10 && curr_token!=11 && curr_token!=21 && curr_token!=22) {
		var=ID();
	} else {
		cout << "Expected variable or '(', found '" << look_up(tokens[curr_position]) << "' at position " << curr_position << "\n";
		throw std::invalid_argument( "ERROR 3" ); 	
	}
	tree_lvl_print();
	tree_lvl--;
	cout << "factor Return\n";	
	return var;
}

Node ID() {
	tree_lvl++;
	tree_lvl_print();
	cout << "ID\n";	
	char temp=look_up(tokens[curr_position]);
	Node var;
	var.data=temp;
	var.op=' ';
	LookAhead();
	tree_lvl_print();
	tree_lvl--;
	cout << "ID Return\n";	
	return var;
}

Node paren() {
	tree_lvl++;
	tree_lvl_print();
	cout << "paren\n";	
	LookAhead();
	Node var = express();
	if(tokens[curr_position] != 32) {
		cout << "Expected ')', found '" << look_up(tokens[curr_position]) << "' at position " << curr_position << "\n";
		throw std::invalid_argument( "ERROR 2" ); 
	}
	tree_lvl_print();
	tree_lvl--;
	cout << "paren Return\n";	    
	LookAhead();
	return var;
}

Node RetainParantheses(Node var) {
	tree_lvl++;
	tree_lvl_print();
	cout << "RetainParantheses\n";
	Node var_return;
	var_return.data="("+var.data+")";
	var_return.op=var.op;
	tree_lvl_print();
	tree_lvl--;
	cout << "RetainParantheses Return\n";	
	return var_return;
}


void print_tokens() {
	cout << "\nNumber of tokens are: " << no_tokens << "\n";
	cout << "\n==================== Tokens ====================\n\n";
	for(int i=0; i<=no_token; i++) {
		cout << tokens[i] << " " << "\n";
	}
	cout << "\n================================================\n";
}


