#if !defined(CONSOLE_ERROR)
#define CONSOLE_ERROR

#include <string>
#include <iostream>
#include <sstream>
class ConsoleError {

private:
	std::string errorMessage;
	std::string calledFunction;
	int lineNumber;
public:
	ConsoleError(std::string error, std::string function, int line) : errorMessage(error), calledFunction(function), lineNumber(line) {}

	std::string ToString() {
		std::stringstream oss;
		oss << "Error Message: " << errorMessage + "\nOccurred in function: " << calledFunction << "\nAt Line Number: " << lineNumber;
		return oss.str();
	}
};


#endif