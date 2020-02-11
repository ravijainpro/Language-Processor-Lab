%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
%}

 // %union {char *str;}
%start line
 // %type <str> exp term factor id
%token VAR OP1 OP2 OP3 OP4 OP5 OP6


%%

line	:	exp											{printf ("1a %c\n", $1);}
	    ;

exp 	:   OP5	OP5 exp OP6 OP6							{printf("2anjani removed '((exp))'\n");}
        |	OP5 exp OP2 exp OP6 OP3 exp 				{printf("2b retained '(a+b)*b'\n");}
		|	OP5 exp OP2 exp OP6 OP4 exp 				{printf("2c retained '(a+b)/b'\n");}
		|	OP5 exp OP1 exp OP6 OP3 exp 				{printf("2d retained '(a-b)*b'\n");}
		|	OP5 exp OP1 exp OP6 OP4 exp 				{printf("2e retained '(a-b)/b'\n");}	
		| 	OP5 exp OP6									{printf("2a removed '(exp)'\n");}
		|   exp OP2 OP5 exp OP2 exp OP6					{printf("2f removed 'a+(c+b)'\n");}
		|   exp OP2 OP5 exp OP1 exp OP6					{printf("2g removed 'a+(c-b)'\n");}
		|	exp OP1 OP5 exp OP2 exp OP6					{printf("2h removed 'a-(c+b)'\n");}
		|	exp OP1 OP5 exp OP1 exp OP6					{printf("2i removed 'a-(c-b)'\n");}
		|	exp OP2 OP5 exp OP3 exp OP6					{printf("2j removed 'a+(c*b)'\n");}
		|	exp OP1 OP5 exp OP3 exp OP6					{printf("2k removed 'a-(c*b)'\n");}
		|	exp OP2 OP5 exp OP4 exp OP6					{printf("2l removed 'a+(c/b)'\n");}
		|	exp OP1 OP5 exp OP4 exp OP6					{printf("2m removed 'a-(c/b)'\n");}
		|	exp OP3 OP5 exp OP3 exp OP6					{printf("2n removed 'a*(c*b)'\n");}
		|	exp OP4 OP5 exp OP4 exp OP6					{printf("2o removed 'a/(c/b)'\n");}
		|	exp OP3 OP5 exp OP4 exp OP6					{printf("2p removed 'a*(c/b)'\n");}
		|	exp OP4 OP5 exp OP3 exp OP6					{printf("2q removed 'a/(c*b)'\n");}
		| 	exp OP2 term								{printf("2s +\n");}
    	| 	exp OP1 term								{printf("2t -\n");}
   		|	term										{printf("2r _\n");}
    	;

term	:	factor										{printf("3a %c\n",$1);}
    	| 	term OP3 factor  							{printf("3b *\n");}
    	| 	term OP4 factor  							{printf("3c /\n");}
    	;

factor	:	id      								    {printf("4a %c\n",$1);}
    	|	OP5 exp OP6     					 	    {printf("4b ( %c ) \n",$2);}
		;
id 		:	VAR											{printf("5a %c\n",$1);}
		|	OP1											{printf("5b %c\n",$1);}
		|	OP2											{printf("5c %c\n",$1);}
		|	OP3											{printf("5d %c\n",$1);}
		|	OP4											{printf("5e %c\n",$1);}
		|	OP5											{printf("5f %c\n",$1);}
		|	OP6											{printf("5g %c\n",$1);}
		;
%%

int main (void) {return yyparse ( );}
