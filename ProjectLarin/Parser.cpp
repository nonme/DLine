#include "Parser.h"

OptionalDouble Parser::parse(std::string& command)
{
	setSource(command);
	c = advance();

	OptionalDouble value = statement();
	return value;
}

OptionalDouble Parser::statement()
{
	OptionalDouble value;
	if (isAlpha(c)) {
		char operand = peekWord();
		if (operand == '=') {
			std::string variable = identifier();
			c = advance();
			match('=');
			OptionalDouble assigment = expression();

			if (constants.find(variable) != constants.end() || math_keywords.find(variable) != math_keywords.end()  || variable == "exit")
				throw DeclarationException();
			else
				variables[variable] = assigment;
		}
		else {
			value = expression();
		}
	}
	else if(c != '#') {
		value = expression();
	}
	return value;
}

OptionalDouble Parser::expression()
{
	return addition();
}

#include <iostream>
OptionalDouble Parser::addition()
{
	OptionalDouble value = multiplication();

	while (c == '+' || c == '-') {
		if (c == '+') {
			c = advance();
			value += multiplication();
		}
		else {
			c = advance();
			value -= multiplication();
		}
	}
	return value;
}

OptionalDouble Parser::multiplication()
{
	OptionalDouble value = unary();

	while (c == '*' || c == '/') {
		if (c == '*') {
			c = advance();
			value *= unary();
		}
		else {
			c = advance();
			value /= unary();
		}
	}
	return value;
}

OptionalDouble Parser::unary() {
	OptionalDouble value;
	
	bool minus = false;
	if (c == '-') {
		match('-');
		value = primary();
		if (value.contains())
			value.setValue(-value.getValue());
		minus = true;
	}
	else {
		value = primary();
	}
	
	if (c == '*' && peek() == '*') {
		match('*');
		match('*');
		OptionalDouble exponent = number();

		if (value.contains() && exponent.contains()) {
			if(!minus)
				value.setValue(std::pow(value.getValue(), exponent.getValue()));
			else {
				value.setValue(std::pow(-value.getValue(), exponent.getValue()));
				value.setValue(-value.getValue());
			}
		}
		c = advance();
	}
	return value;
}

OptionalDouble Parser::primary()
{
	OptionalDouble value;
	if (c == '(') {
		c = advance();
		value = expression();
		match(')');
		return value;
	}
	else if (isDigit(c)) {
		value = number();
		c = advance();
	}
	else if (isAlpha(c)) {
		std::string variable = identifier();
		if (math_keywords.find(variable) != math_keywords.end()) {
			c = advance();
			match('(');

			value = expression();
			if (value.contains()) {
				if (variable == "sqrt" && value.getValue() < 0)
					throw InvalidArgument("You can pass only non-negative arguments to 'sqrt' function");
				else if ((variable == "acos" || variable == "asin") && (value.getValue() < -1 || value.getValue() > 1))
					throw InvalidArgument("'acos' and 'asin' accept arguments only in range [-1; 1]");
				else 
					value = math_keywords[variable](value.getValue());
			}

			match(')');
		}
		else if (constants.find(variable) != constants.end()) {
			value.setValue(constants[variable]);
			c = advance();
		}
		else if (variables.find(variable) == variables.end())
			throw SyntaxException("Unknown identifier", current);
		else {
			value = variables[variable];
			c = advance();
		}
	}
	else {
		throw SyntaxException("Uknown identifier", current);
	}
	return value;
}

OptionalDouble Parser::number()
{
	start = current - 1;

	while (isDigit(peek())) advance();

	if (peek() == '.' && isDigit(peekNext())) {
		advance();

		while (isDigit(peek())) advance();
	}

	std::string str_value = source.substr(start, current - start);
	OptionalDouble value;
	try {
		value.setValue(std::stod(str_value));
	}
	catch (std::invalid_argument e) {
		throw SyntaxException("Error occured while parsing the number", start);
	}
	catch (std::out_of_range e) {
		throw SyntaxException("The number is too long.", start);
	}

	return value;
}

std::string Parser::identifier()
{
	start = current - 1;

	while (isAlphaNumeric(peek())) c = advance();

	std::string text = source.substr(start, current - start);

	return text;
}

void Parser::setSource(std::string& source)
{
	//source.erase(std::remove_if(source.begin(), source.end(), std::isspace), source.end());

	this->source = source;
	current = 0;
	start = 0;
}

char Parser::advance()
{
	current++;
	while (source[current - 1] == ' ' && current < source.size()) current++;
	return source[current - 1];
}


bool Parser::isAtEnd()
{
	return current >= source.size();
}

bool Parser::match(char expected)
{
	if (isAtEnd())
		return false;
	if (c != expected) {
		std::string str = "Expected ";
		str += expected;
		throw SyntaxException(str, current);
	}
	c = advance();
	return true;
}

char Parser::peek()
{
	if (isAtEnd())
		return '\0';
	return source[current];
}

bool Parser::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

char Parser::peekNext()
{
	if (current + 1 >= source.length()) return '\0';
	return source[current + 1];
}

char Parser::peekWord()
{
	int peek = current;
	while (peek < source.size() && (isAlphaNumeric(source[peek]) || source[peek] == ' ')) peek++;
	if (peek >= source.size())
		return '\0';
	else
		return source[peek];
}

bool Parser::isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Parser::isAlphaNumeric(char c)
{
	return isDigit(c) || isAlpha(c);
}