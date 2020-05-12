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
void ID(string*, char*);
void RetainParantheses(string*, char*, string*, char*);
void paren(string*, char*);
void factor(string*, char*);
void term(string*, char*);
void express(string*, char*);
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
	++curr_position;
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
	string var;
	char var_op;
	express(&var, &var_op);
	cout  <<"return start_line:";
	if(curr_position>no_token) {
		cout << "RETURN\n\n\n";
		tree_lvl_print();
		tree_lvl--;
		return var;
	}
	cout << "\n\n\n";
	tree_lvl_print();
	tree_lvl--;
	return var;
}

void express(string *to_return, char *to_return_op) {
	tree_lvl++;
	tree_lvl_print();	
	cout << "express\n";
	string Lside_var;
	char Lside_var_op;
	term(&Lside_var, &Lside_var_op);
	int op=tokens[curr_position];
	if (op != 10 && op != 11) {
		*to_return=Lside_var;
		*to_return_op=Lside_var_op;
		tree_lvl_print();
		tree_lvl--;
		cout << "express Lside Return\n";	
	    return;
	}
	cout << "+|- Encountered\n";
    LookAhead();
	string Rside_var;
	char Rside_var_op;
	term(&Rside_var, &Rside_var_op);
    *to_return=Lside_var + look_up(op) + Rside_var;
    *to_return_op = look_up(op);
	tree_lvl_print();
	tree_lvl--;
    cout << "express Return\n";    
}

void term(string *to_return, char *to_return_op) {
	tree_lvl++;
	tree_lvl_print();
	cout << "term\n";
	string Lside_var;
	char Lside_var_op;
	factor(&Lside_var, &Lside_var_op);
	*to_return=Lside_var;
	*to_return_op=Lside_var_op;
	if(curr_position>no_token) {
		tree_lvl_print();
		tree_lvl--;
		cout << "\nRETURN\n";	
	    return;
	}
	int op=tokens[curr_position];
	if(op!=21 && op!=22) {
		tree_lvl_print();
		tree_lvl--;
		cout << "term Lside Return\n";	
	    return;
	}
	cout << "*|/ Encountered\n";
	LookAhead();
	string Rside_var;
	char Rside_var_op;
	factor(&Rside_var, &Rside_var_op);
	bool a=false, b=false;
	string new_var_a;
	char new_var_a_op;
	string new_var_b;
	char new_var_b_op;	
	if(Lside_var_op == '+' || Lside_var_op == '-') {
		a=true;
		RetainParantheses(&Lside_var, &Lside_var_op, &new_var_a, &new_var_a_op);
	}
	if(Rside_var_op == '+' || Rside_var_op == '-') {
		b=true;
		RetainParantheses(&Rside_var, &Rside_var_op, &new_var_b, &new_var_b_op);
	}
    *to_return_op = look_up(op);
    if(a && b) {
    	*to_return = new_var_a + look_up(op) + new_var_b;	
    }
    else if(a && !b) {
    	*to_return = new_var_a + look_up(op) + Rside_var;
    }
    else {
    	*to_return = Lside_var + look_up(op) + new_var_b;
    }
   	tree_lvl_print();
	tree_lvl--;
    cout << "term Return\n";	
}

void factor(string *to_return, char *to_return_op) {
	tree_lvl++;
	tree_lvl_print();
	cout << "factor\n";	
	int curr_token=tokens[curr_position];
	if(curr_token==31) {
		paren(to_return, to_return_op);
	} 
	else if(curr_token!=31 && curr_token!=32 && curr_token!=10 && curr_token!=11 && curr_token!=21 && curr_token!=22) {
		ID(to_return, to_return_op);
	}
	tree_lvl_print();
	tree_lvl--;
	cout << "factor Return\n";	
}

void ID(string *to_return, char *to_return_op) {
	tree_lvl++;
	tree_lvl_print();
	cout << "ID\n";	
	char temp=look_up(tokens[curr_position]);
	*to_return=temp;
	*to_return_op=' ';
	LookAhead();
	tree_lvl_print();
	tree_lvl--;
	cout << "ID Return\n";
}

void paren(string *to_return, char *to_return_op) {
	tree_lvl++;
	tree_lvl_print();
	cout << "paren\n";	
	LookAhead();
	express(to_return, to_return_op);
	if(tokens[curr_position] != 32) {
		throw std::invalid_argument( "Expected ')'\n" ); 
	}
	tree_lvl_print();
	tree_lvl--;
	cout << "paren Return\n";	    
	LookAhead();
}

void RetainParantheses(string *var, char *op, string *to_return, char *to_return_op) {
	tree_lvl++;
	tree_lvl_print();
	cout << "RetainParantheses\n";
	*to_return="("+*var+")";
	*to_return_op=*op;
	tree_lvl_print();
	tree_lvl--;
	cout << "RetainParantheses Return\n";	
}


void print_tokens() {
	cout << "\nNumber of tokens are: " << no_tokens << "\n";
	cout << "\n==================== Tokens ====================\n\n";
	for(int i=0; i<=no_token; i++) {
		cout << tokens[i] << " " << "\n";
	}
	cout << "\n================================================\n";
}


