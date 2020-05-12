#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#define A 65
#define B 66
#define C 67
#define D 68
#define E 69
#define F 70
#define G 71
#define H 72
#define I 73
#define OP1 10 //plus
#define OP2 11 //minus
#define OP3 21 //multipication
#define OP4 22 //division
#define OP5 31 //'('
#define OP6 32 //')'
char symbol_table[15][2];
char input_buffer[15];
int no_tokens;
char buffer;
bool flag=true;
int sym_ptr=-1;

void install_token(char token) {
	bool check=true;
	if(sym_ptr!=-1) {
		for(int i=0; i<sym_ptr; i++) {
			if(symbol_table[i][0]==token) {
				check=false;
			}
		}
	}
	if(check) {
		symbol_table[++sym_ptr][0]=token;
		switch(token) {
			case 'A': symbol_table[sym_ptr][1]='1'; 
					  break;
			case 'B': symbol_table[sym_ptr][1]='1';
					  break;
			case 'C': symbol_table[sym_ptr][1]='1';
					  break;
			case 'D': symbol_table[sym_ptr][1]='1';
					  break;
			case 'E': symbol_table[sym_ptr][1]='1';
					  break;
			case 'F': symbol_table[sym_ptr][1]='1';
					  break;
			case 'G': symbol_table[sym_ptr][1]='1';
					  break;
			case 'H': symbol_table[sym_ptr][1]='1';
					  break;
			case 'I': symbol_table[sym_ptr][1]='1';
					  break;
			case '+': symbol_table[sym_ptr][1]='2';
					  break;
			case '-': symbol_table[sym_ptr][1]='2';
					  break;
			case '*': symbol_table[sym_ptr][1]='2';
					  break;
			case '/': symbol_table[sym_ptr][1]='2';
					  break;
			case '(': symbol_table[sym_ptr][1]='3';
					  break;
			case ')': symbol_table[sym_ptr][1]='3';
					  break; 
		}
	}
}

void print_symbol_table() {
	cout << "\n\n==================Symbol Table==================\n\n";
	cout << "SYMBOL | TYPE\n";
	for(int i=0; i<=sym_ptr; i++) {
		cout << symbol_table[i][0] << "      |      " << symbol_table[i][1] << "\n";
	}
	cout << "\n\n================================================\n\n";
}

void input() {
	cin >> input_buffer;
}

int tokens_check(int *to_return) {
	no_tokens=-1;
	int input_buffer_length=strlen(input_buffer);
	while(input_buffer_length) {
		buffer=input_buffer[++no_tokens];
		install_token(buffer);
		switch(buffer) {
			case 'A': to_return[no_tokens]=A;
 					  break;
			case 'B': to_return[no_tokens]=B;
   				 	  break;
			case 'C': to_return[no_tokens]=C;
   				 	  break;
			case 'D': to_return[no_tokens]=D;
   				 	  break;
			case 'E': to_return[no_tokens]=E;
   				 	  break;
			case 'F': to_return[no_tokens]=F;
   				 	  break;
			case 'G': to_return[no_tokens]=G;
   				 	  break;
			case 'H': to_return[no_tokens]=H;
   				 	  break;
			case 'I': to_return[no_tokens]=I;
   				 	  break;
			case '+': to_return[no_tokens]=OP1;
   				 	  break;
			case '-': to_return[no_tokens]=OP2;
   				 	  break;
			case '*': to_return[no_tokens]=OP3;
   				 	  break;
			case '/': to_return[no_tokens]=OP4;
   				 	  break;
			case '(': to_return[no_tokens]=OP5;
   				 	  break;
			case ')': to_return[no_tokens]=OP6;
   				 	  break;
			default: flag=false;
 		}
		if(!flag) {
			cout << "\n\nLex Error\n\n";
			return 0;
		}
		input_buffer_length--;
	}
	return 1;
}

int return_no_token() {
	return no_tokens;
}
