#pragma once
#include <string>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "OptionalDouble.h"

class Parser {
private:
	std::unordered_map<std::string, OptionalDouble> variables;
	typedef double(*func_ptr)(double);
	std::unordered_map<std::string, func_ptr> math_keywords;
	std::unordered_map<std::string, double> constants;

	int current = 0;
	int start = 0;
	char c;
	std::string source;

	bool isAtEnd();
	char advance();
	bool match(char expected);
	char peek();
	bool isDigit(char c);
	OptionalDouble number();
	char peekNext();
	char peekWord();
	std::string identifier();
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);
	void setSource(std::string& source);
	
	OptionalDouble statement();
	OptionalDouble expression();
	OptionalDouble addition();
	OptionalDouble multiplication();
	OptionalDouble unary();
	OptionalDouble primary();
public:
	Parser() {
		c = '\0';
		source = "";

		math_keywords["cos"] = &std::cos;
		math_keywords["sin"] = &std::sin;
		math_keywords["tan"] = &std::tan;
		math_keywords["ln"]  = &std::log;
		math_keywords["log"] = &std::log2;
		math_keywords["abs"] = &std::abs;
		math_keywords["sqrt"] = &std::sqrt;
		math_keywords["acos"] = &std::acos;
		math_keywords["asin"] = &std::asin;
		math_keywords["atan"] = &std::atan;

		constants["PI"] = 3.14159265359;
		constants["E"]  = 2.7182818284;
		constants["pi"] = 3.14159265359;
		constants["e"] = 2.7182818284;
	};
	OptionalDouble parse(std::string& command);
};