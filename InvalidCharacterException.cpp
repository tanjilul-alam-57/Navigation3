
#include "InvalidCharacterException.h"
#include <string>
#include <sstream>

using namespace std;

InvalidCharacterException::InvalidCharacterException(std::string character,
		int line):runtime_error(getException(character,line))
{

}

const string InvalidCharacterException::getException(string character, int line)
{
	ostringstream stream;
	stream<<line;
	string lineNum = stream.str();
	stream.str("");
	stream.clear();
	string exception = "Exception: Invalid Character '"+character+"' found in line <"+lineNum+">";
	return exception;
}



