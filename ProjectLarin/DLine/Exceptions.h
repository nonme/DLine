#pragma once

#include <exception>
#include <string>

class BadOptionalAccess : public std::exception {
public:
	BadOptionalAccess() {};
	const char* what() const throw () {
		return "The optional value is inacessible.";
	}
};

class DeclarationException : public std::exception {
public:
	DeclarationException() {};
	const char* what() const throw () {
		return "The identifier is already reserved word. Try another name for your variable.";
	}
};

class InvalidArgument : public std::exception {
private:
	std::string error;
public:
	InvalidArgument(std::string error) : error(error) {};
	const char* what() const throw () {
		return error.c_str();
	}
};

class SyntaxException : public std::exception {
private:
	int position;
	std::string message;
	std::string error;
public:
	SyntaxException(std::string message, int position) : message(message), position(position) {
		error = std::to_string(position) + ": " + message;
	};
	const char* what() const throw () {
		return error.c_str();
	}
};