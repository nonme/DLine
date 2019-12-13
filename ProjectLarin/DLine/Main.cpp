#include <iostream>
#include <string>
#include <fstream>

#include "Parser.h"
#include "OptionalDouble.h"


int main(int argc, char* argv[]) {
	std::cout << "Welcome to DLine!" << std::endl;
	std::cout << "Type Ctrl+Z or 'exit' to stop accepting commands." << std::endl;

	Parser parser;
	if (argc > 2) {
		std::cout << "Too many arguments provided!";
	}
	else if (argc == 2) {
		std::ifstream file(argv[1]);
		if (!file.is_open()) {
			std::cout << "File not found";
		}
		else {
			std::string command;
			while (std::getline(file, command)) {
				std::cout << command << std::endl;
				try {
					OptionalDouble value = parser.parse(command);
					if (value.contains())
						std::cout << "=> " << value.getValue() << std::endl;
				}
				catch (SyntaxException e) {
					std::cout << e.what() << std::endl;
					break; // Dear E. W. Dijkstra, I apologize
				}
				catch (DeclarationException e) {
					std::cout << e.what() << std::endl;
					break;
				}
				catch (BadOptionalAccess e) {
					std::cout << e.what() << std::endl;
					break;
				}
				catch (InvalidArgument e) {
					std::cout << e.what() << std::endl;
				}
			}
			file.close();
		}
	}
	std::string command = "";
	std::cout << ">>> ";
	while (std::getline(std::cin, command) && command != "exit") {
		try {
			OptionalDouble value = parser.parse(command);
			if (value.contains())
				std::cout << "=> " << value.getValue() << std::endl;
		}
		catch (SyntaxException e) {
			std::cout << e.what() << std::endl;
		}
		catch (DeclarationException e) {
			std::cout << e.what() << std::endl;
		}
		catch (BadOptionalAccess e) {
			std::cout << e.what() << std::endl;
		}
		catch (InvalidArgument e) {
			std::cout << e.what() << std::endl;
		}
   		std::cout << ">>> ";
	}
}