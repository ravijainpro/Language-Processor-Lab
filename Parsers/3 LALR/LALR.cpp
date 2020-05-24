#include <cctype>
#include <cstdarg>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include<stdlib.h>
#include<cstring>
struct Token{
	enum TokenType{
		S1='Z',
		S='S',
		C='C',
		d='d',
		c='c',
		END=1,
		INTEGER='0',
		null=0,
		expr=128
	} type;
	double intValue;
	Token(TokenType type=END):type(type),intValue(0){}
	Token(long val):type(INTEGER),intValue(val){}
	Token(char character){
		this->type=(TokenType)character;
	}
};

struct Rule{
	Token reduces_to;
	std::vector<Token> constraints;
	Token lookahead;
	Rule(const Token &to,const Token &la,unsigned constraints,...){
		this->reduces_to=to;
		this->lookahead=la;
		va_list list;
		va_start(list,constraints);
		this->constraints.reserve(constraints);
		for (unsigned a=0;a<constraints;a++)
			this->constraints.push_back(va_arg(list,Token::TokenType));
	}
	bool matches(const std::vector<Token> &stack,const Token &lookahead){
		if (stack.size()<this->constraints.size() ||
				this->lookahead.type!=Token::null && this->lookahead.type!=lookahead.type)
			return 0;
		const Token *array=&stack[stack.size()-this->constraints.size()];
		for (unsigned a=0,size=this->constraints.size();a<size;a++)
			if (array[a].type!=this->constraints[a].type)
				return 0;
		return 1;
	}
};

class Parser{
	std::stringstream stream;
	std::vector<Token> stack;
	bool result;
	std::vector<Rule> rules;
	Token read(){
		char character;
		while (!this->stream.eof() && isspace(character=this->stream.peek()))
			this->stream.get();
		if (this->stream.eof())
			return Token::END;
		character=this->stream.peek();
		if (isdigit(character)){
			std::string str;
			str.push_back(this->stream.get());
			while (isdigit(this->stream.peek()))
				str.push_back(this->stream.get());
			long temp=atoi(str.c_str());
			return temp;
		}
		return (char)this->stream.get();
	}
		bool reduce(const Token &lookahead){
		long rule_index=-1;
		unsigned max=0;
		for (unsigned a=0;a<this->rules.size();a++){
			if (this->rules[a].matches(this->stack,lookahead) && this->rules[a].constraints.size()>max){
				rule_index=a;
				max=this->rules[a].constraints.size();
			}
		}
		if (rule_index<0 || this->rules[rule_index].reduces_to.type==Token::null)
			return 0;
		Rule &rule=this->rules[rule_index];
		Token new_token(rule.reduces_to);
		Token *redex=&this->stack[this->stack.size()-rule.constraints.size()];
		switch (rule_index){
			case 0: //expr <- INTEGER
				new_token.intValue=redex[0].intValue;
				break;
			case 1: //expr <- '(' expr ')'
			case 2: //expr <- '+' expr
				new_token.intValue=redex[1].intValue;
				break;
			case 3: //expr <- '-' expr
				new_token.intValue=-redex[1].intValue;
				break;
			case 4: //impossible
			case 5: //expr <- expr '^' expr
				new_token.intValue=pow((double)redex[0].intValue,(double)redex[2].intValue);
				break;
			case 6: //expr <- expr '*' expr
				new_token.intValue=redex[0].intValue*redex[2].intValue;
				break;
			case 7: //expr <- expr '/' expr
				new_token.intValue=redex[0].intValue/redex[2].intValue;
				break;
			case 8: //impossible
			case 9: //impossible
			case 10: //expr <- expr '+' expr
				new_token.intValue=redex[0].intValue+redex[2].intValue;
				break;
			case 11: //impossible
			case 12: //impossible
			case 13: //expr <- expr '-' expr
				new_token.intValue=redex[0].intValue-redex[2].intValue;
				break;
		}
		for (unsigned a=0;a<rule.constraints.size();a++)
			this->stack.pop_back();
		this->stack.push_back(new_token);
		return 1;
	}
	bool run_state(){
		Token next_token=this->read();
		while (this->reduce(next_token));
		switch (next_token.type){
			case Token::END:
				this->result=(this->stack.size()==1);
				return 0;
			case Token::INTEGER:
			case Token::c:
			case Token::d:
				this->stack.push_back(next_token);
				return 1;
			default:
				this->result=0;
				return 0;
		}
	}
	void initializeRules(){
		this->rules.clear();
		/*rule 0*/	this->rules.push_back(Rule(	Token::expr,	Token::S1,	1,	Token::S	));

		/*rule 1*/	this->rules.push_back(Rule(	Token::expr,	Token::S,	2,	Token::C,	Token::C));

		/*rule 2*/	this->rules.push_back(Rule(	Token::expr,	Token::C,	2,	Token::c,	Token::C	));
		/*rule 3*/	this->rules.push_back(Rule(	Token::expr,	Token::C,	1,	Token::d));

	}
public:
	Parser(const std::string &str)
			:stream(str){
		this->result=0;
		this->initializeRules();
	}
	bool eval(double &res){
		while (this->run_state());
		if (this->result)
			res=this->stack.front().intValue;
		else
			this->stack.clear();
		return this->result;
	}
};

int main(){
	std::string expr;
	std::cout<<"Enter a expression to Evaluate: ";
	std::cin>>expr; 
	Parser evaluator(expr);
	double res=0;
	if (evaluator.eval(res)){
		std::cout<<"ACCEPTED";
	}
	else{
		std::cout<<"REJECTED";
	}
	//std::cout <<(evaluator.eval(res)?"ACCEPT":"ABORT")<<std::endl;
	//std::cout <<res<<std::endl;
	return 0;
}
