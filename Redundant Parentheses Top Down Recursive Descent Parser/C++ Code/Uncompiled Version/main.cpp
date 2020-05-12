#include "parser1.h"

int main() {
	input();
	init_tokens();
	if(tokens_check(tokens)) {
		no_token=return_no_token();
		print_tokens();
		print_symbol_table();
		cout << "\nLex work is done!\n";
	}
	else {
		cout << "\nInvalid input string\n";
	}
	cout << "\nRESULT: " << start_line() << "\n";
	return 0;
}