#include "lex.h"
#include <stdexcept>
#define LENGTH 100

struct Node {
	string data;
	char op;
};
void nextToken();
char look_up(int);
Node ID();
Node RetainParantheses(Node);
Node paren();
Node factor();
Node term();
Node expression();
string start_line();
void print_tokens();



int tokens[LENGTH];
int no_token;
int lookahead;
int curr_position=-1; 

void init_tokens() {
	cout << "\ni1 nit_token\n";
	for(int i=0; i<LENGTH; i++) {
		tokens[i]=-1;
	}
}

void nextToken() {
	cout << "\n2 nextToken\n";	
	if(curr_position > no_token) {
		cout << "END OF INPUT IS NOT ACCEPTED\n";
		throw std::invalid_argument( "ERROR 4" ); 
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
	cout << "\n3 look_up\n";
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

Node ID() {
	cout << "\n4 ID\n";	
	char temp=look_up(tokens[curr_position]);
	Node var;
	var.data=temp;
	var.op=' ';
	nextToken();
	return var;
}

Node RetainParantheses(Node var) {
	cout << "\n5 RetainParantheses\n";
	Node var_return;
	var_return.data="("+var.data+")";
	var_return.op=var.op;
	return var_return;
}

Node paren() {
	cout << "\n6 paren\n";	
	nextToken();
	Node var = expression();
	if(tokens[curr_position] != 32) {
		cout << "Expected ')', found '" << look_up(tokens[curr_position]) << "' at position " << curr_position << "\n";
		throw std::invalid_argument( "ERROR 1" ); 
	}
	nextToken();
	return var;
}

Node factor() {
	cout << "\n7 factor\n";	
	Node var;
	int curr_token=tokens[curr_position];
	if(curr_token==31) {
		var = paren();
	} else if(curr_token!=31 && curr_token!=32 && curr_token!=10 && curr_token!=11 && curr_token!=21 && curr_token!=22) {
		var=ID();
	} else {
		cout << "Expected variable or '(', found '" << look_up(tokens[curr_position]) << "' at position " << curr_position << "\n";
		throw std::invalid_argument( "ERROR 2" ); 	
	}
	return var;
}

Node term() {
	cout << "\n8 term\n";	
	Node leftArg = factor();
	if(curr_position>no_token) {
		return leftArg;
	}
	int op=tokens[curr_position];
	if(op!=21 && op!=22) {
		return leftArg;
	}
	nextToken();
	Node rightArg = factor();
	if(leftArg.op == '+' || leftArg.op == '-') {
		leftArg = RetainParantheses(leftArg);
	}
	if(rightArg.op == '+' || rightArg.op == '-' || (op == 21 && (rightArg.op == '/' || rightArg.op == '*'))) {
            rightArg = RetainParantheses(rightArg);
    }
    Node to_return;
    to_return.data = leftArg.data + " " + look_up(op) + " " + rightArg.data;
    to_return.op = look_up(op);
    return to_return;
}

Node expression() {
	cout << "\n9 expression\n";	
	Node leftArg = term();
	int op=tokens[curr_position];
	if (op != 10 && op != 11) {
	    return leftArg;
	}
    nextToken();
	Node rightArg = term();
    if(op == 10 && (rightArg.op == '-' || rightArg.op == '+')) {
        rightArg = RetainParantheses(rightArg);
    }
	Node to_return;
    to_return.data=leftArg.data + " " + look_up(op) + " " + rightArg.data;
    to_return.op = look_up(op);
    return to_return;    
}

string start_line() {
	cout << "\n10 start_line\n";
	nextToken();
	Node var = expression();
	if(curr_position>no_token) {
		return var.data;
	}
	if(tokens[curr_position]==-1) {
		cout << "Found unexpected character '" << look_up(tokens[curr_position]) << "' at position " << curr_position << "\n";
		throw std::invalid_argument( "ERROR 3" ); 	
	}
	return var.data;
}


//below funcation is just for debugging purpose
void print_tokens() {
	cout << "\nNumber of tokens are: " << no_token << "\n";
	cout << "\n==================== Tokens ====================\n";
	for(int i=0; i<=no_token; i++) {
		cout << tokens[i] << " " << "\n";
	}
	cout << "\n================================================\n";
}

int main() {
	input();
	init_tokens();
	if(tokens_check(tokens)) {
		no_token=return_no_token();
		print_tokens();
		cout << "\nLex work is done!\n";
	}
	else {
		cout << "\nInvalid input string\n";
	}
	cout << start_line() << "\n";
	return 0;
}



