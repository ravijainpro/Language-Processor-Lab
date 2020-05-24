#include <bits/stdc++.h>
#define INVAL -999
using namespace std;

//input data
char fun[]="insert(int m[], int i, int data)";
char For[]="For(j=i-1;i≥0;j=j-1)";
char iFElse[]="if m[j] < data then m[j+1]=m[j] else break; m[j+1] = data;";

//symbol table
struct symbolTable {
	string name;
	unordered_map<string, int> symbols; 
	//we are only dealing with int types; so no need to add complexity by adding multiple attributes to the table.
};

//stack to hold the list of symbol tables..
stack <symbolTable> symbol_tables;

//stack to hold the current activation frame
stack <symbolTable*> curr_act_frame;

//stack to hold the current offset
stack <int> offset;

//create symbol table 
//as the given code snippet the vals aren't defined..So, this function won't be in action
symbolTable createSymbolTable(string name) {
	symbolTable symTbl;
	symTbl.name=name;
	return symTbl;
}

int coded_lines=0; //to keep track of last line coded..zero implies not yet started..
void emit(string s) {
	if(coded_lines<offset.top()) {
		cout << "\t" << ++coded_lines << ": ";
	}
	if(s!="") {
		cout << s << " ";
	}
}

//look up operation in the current active symbol table
int lookup(string name) { //DECIDE THE RETURN TYPE
	symbolTable *act_sym_tbl=curr_act_frame.top();
	//int t=INVAL; 
	if ((*act_sym_tbl).symbols.find(name)==(*act_sym_tbl).symbols.end()) {
		return 0; //entry not found
	}
	return 1; 
}

//to enter vals in current active symbol table 
// current activation record checked for the same
void entry(string name, int val) {
	symbolTable *act_sym_tbl=curr_act_frame.top();
	int t=INVAL;
	t=lookup(name); 
	if (t!=INVAL) {
		return; //if the entry is already present witht that name then just returnt he call
	}
	(*act_sym_tbl).symbols[name]=val;
}



//if else SDT => Generates intermediate code for if else 
void ifElseSDT(char input[]) {
	cout <<"\n";
	char *temp=input;
	cout << "F3:";
	emit("");
	bool inital=true, inner=true, last=true; //flags for scanning
	while(inital && inner && last) {
		while(inital) {
			cout << (*temp) ;
			temp++;
			if((*temp)=='t' && (*(temp+1))=='h' && (*(temp+2))=='e' && (*(temp+3))=='n') {
				emit("goto ");
				temp+=5;
				cout << offset.top()+2 << "\n";	
				inital=false;
			}	
		}	
		offset.push(offset.top()+1);
		emit("goto L1");
		cout << "\n";
		offset.push(offset.top()+1);
		string t1="t2=",t2="t3="; //assuming they are registers
		char symbol=(*temp); //assuming varibales to be of length 1		
		temp++;
		emit("t1=addr(");
		cout << symbol << " )";
		offset.push(offset.top()+1);
		cout << "\n";
		
		if((*temp)=='[' && inner==true) {
			inner=false;
			temp++;
			while((*temp)!=']') {
				t1.push_back(*temp);
				temp++;
			}
			emit(t1);
			offset.push(offset.top()+1);
			temp++;
			//after scanning let j be stored in t2
			while((*temp)!=']') {
				temp++;
				if((*temp)=='[') {
					temp++;
					t2.push_back(*temp);
				}
			}
			cout << "\n";
			emit(t2);
			cout << "\n";
			offset.push(offset.top()+1);
			emit("");
			//print the final of it
			cout << "t1[t2]=t1[t3]\n";
			offset.push(offset.top()+1);
			emit("goto L2");	
			offset.push(offset.top()+1);
			cout <<"\n";
			while((*temp)!='b' && (*(temp+1)!='r') && (*(temp+2)!='e') && (*(temp+3)!='a') && (*(temp+1)!='k')) {
				temp++;
			}
			temp+=14; //"else" accounts for 4 char + "break" for 5 + 5 wide space => net 9
			cout << "L1:";
			emit("goto EXIT");
			offset.push(offset.top()+1);
		}
		temp++;

		if(last==true && (*temp)=='[') {
			last=false;
			temp++;
			string t3="t4=",t4="t5="; //registers
			while((*temp)!=']') {
				t3.push_back(*temp);
				temp++;
			}
			temp++;
			cout << "\nL2:";
			emit(t3);
			offset.push(offset.top()+1);
			temp+=2;
			while((*temp)!=';'){
				t4.push_back(*temp);
				temp++;
			}
			cout << "\n";
			emit(t4);
			offset.push(offset.top()+1);
			cout << "\n";
			emit("t1[t4]=t5");
		}
	temp++;
	}

}


//"for loop" STD => Generates intermediate code for "for loop"
void forLoopSDT(char input[]) {
	//for(j=i-1; i≥0; j=j-1) {set of statements}
	char *temp=input;
	while((*temp)!='\0') {
		if ((*temp)=='(') {
			temp++;
			emit("");
			while((*temp!=';')) {
				cout << *temp; 
				temp++;
			}
			cout << "\n";
			offset.push(offset.top()+1);
			temp++;
			cout << "F1:";
			emit("if");
			while((*temp)!=';') {
				cout << *temp;
				temp++;
			}
			emit(" goto F3");
			cout << "\n";
			temp++;
			offset.push(offset.top()+1);
			emit("goto EXIT\nF2:");
			offset.push(offset.top()+1);
			emit("");
			while((*temp)!=')') {
				cout << *temp;
				temp++;
			}
			int to_return_at=offset.top();
			offset.push(offset.top()+1);
			cout << "\n";
			emit("goto F1");
			offset.push(offset.top()+1);
			//if else encountered
			ifElseSDT(iFElse);
			offset.push(offset.top()+1);
			cout << "\n";
			emit("goto");
			cout <<"F2\n";
			break;
		}
		temp++;
	}
}

//function call STD  => generates intermediate code output for function calls.
void FunCallSDT(char input[]) {
	int offset_val=offset.top();
	char *temp=input;
	int no_params=0;
	string args[10];
	string fun_name="";
	for(int i=0; i<10; i++) {
		args[i]=""; //to initalise it
	}
	//to obatin fun name from the given input
	while(true) {
		fun_name.push_back(*temp);
		temp++;
		if((*temp)=='(') {
			break;
		}
	}
	//to fetch all the given parameters
	while(true) {
		temp++;
		bool end_check=true, trim_extra_spaces=false;
		while(true) {
			if((*temp)==',' || (*temp)==')') {
				if((*temp)==')') {
					end_check=false;
				}
				break;
			}
			if(*temp!=' ') {
				trim_extra_spaces=true;
			}
			if(trim_extra_spaces) {
				args[no_params].push_back(*temp);
			}
			temp++;
		}
		if(end_check==false) {
			break;
		}
		no_params++;
	}
	for(int i=0; i<=no_params; i++) {
		emit("Param");
		cout << i+1 << ": " ;
		emit(args[i]);
		offset.push(1+offset.top());
		cout << "\n";
	}
	emit("call ");
	emit(fun_name);
	offset.push(1+offset.top());
	cout << "\n";
	forLoopSDT(For);
	emit("EXIT: ");
	offset.push(1+offset.top());
	emit("Return");
	
}

//to simulate intermediate code generation
void generate_code() {
	FunCallSDT(fun);

	
	cout << "\n";
}

int main() {
	offset.push(1);
	generate_code();
	return 0;
}