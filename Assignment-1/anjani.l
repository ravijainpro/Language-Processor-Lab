
%{
#include"y.tab.h"
void yyerror(char *);
%} 

%%
[ \t\n]               {return 0;}
[A-Za-z]              {yylval = *yytext; return VAR;}
[-]              		  {yylval= *yytext; return OP1;}
[+]              		  {yylval= *yytext; return OP2;}
[*]              		  {yylval= *yytext; return OP3;}
[/]              		  {yylval= *yytext; return OP4;}
[(]             		  {yylval= *yytext; return OP5;}
[)]              		  {yylval= *yytext; return OP6;}
.                     {ECHO;
 					   yyerror ("unexpected character");
 					  }

%%

int yywrap (void) {
	return 1;
}

void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}


/*
|	OP5 exp OP2 exp OP6 OP3 exp 				{printf("2b retained '(a+b)*b'\n");}
		|	OP5 exp OP2 exp OP6 OP4 exp 				{printf("2c retained '(a+b)/b'\n");}
		|	OP5 exp OP1 exp OP6 OP3 exp 				{printf("2d retained '(a-b)*b'\n");}
		|	OP5 exp OP1 exp OP6 OP4 exp 				{printf("2e retained '(a-b)/b'\n");}
		

//removed 
|	OP5 exp OP2 exp OP6 OP3 exp 				{printf("2b retained '(a+b)*b'\n");}
		|	OP5 exp OP2 exp OP6 OP4 exp 				{printf("2c retained '(a+b)/b'\n");}
		|	OP5 exp OP1 exp OP6 OP3 exp 				{printf("2d retained '(a-b)*b'\n");}
		|	OP5 exp OP1 exp OP6 OP4 exp 				{printf("2e retained '(a-b)/b'\n");}
		



exp OP3 OP5 exp OP2 exp OP6					{printf("2a retained 'a*(c+b)'\n");}
		|	exp OP3 OP5 exp OP1 exp OP6					{printf("2b retained 'a*(c-b)'\n");}
		|	exp OP4 OP5 exp OP2 exp OP6					{printf("2c retained 'a/(c+b)'\n");}
		|	exp OP4 OP5 exp OP1 exp OP6					{printf("2d retained 'a/(c-b)'\n");}
	*/