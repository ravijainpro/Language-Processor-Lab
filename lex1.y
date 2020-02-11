%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
%}

%token VAR OP1 OP2 OP3 OP4 OP5 OP6
 //%union {char a_char;}
%start line

 //type <a_char> exp term factor id


%%

line	:	exp					{printf ("1a %c\n", $1);}
	    ;

exp 	:	term				{printf("2a _\n");}
    	| 	exp OP2 term			{printf("2b +\n");}
    	| 	exp OP1 term			{printf("2c -\n");}
    	;

term	:	factor				{printf("3a %c\n",$1);}
    	| 	term OP3 factor  		{printf("3b *\n");}
    	| 	term OP4 factor  		{printf("3c /\n");}
    	;

factor	:	OP5 exp OP6      	    {printf("4a ( %c ) \n",$2);}
		|	id      		    {printf("4b %c\n",$1);}
    	;
id: VAR			{printf("5a %c\n",$1);}
	|OP1			{printf("5b %c\n",$1);}
	|OP2			{printf("5c %c\n",$1);}
	|OP3			{printf("5d %c\n",$1);}
	|OP4			{printf("5e %c\n",$1);}
	|OP5			{printf("5f %c\n",$1);}
	|OP6			{printf("5g %c\n",$1);}
	;
%%

int main (void) {return yyparse ( );}
