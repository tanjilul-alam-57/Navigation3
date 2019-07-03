
#ifndef INVALIDCHARACTEREXCEPTION_H_
#define INVALIDCHARACTEREXCEPTION_H_

#include <exception>
#include <string>
#include <stdexcept>

//using std::runtime_error;

class InvalidCharacterException :public std::runtime_error
{
private:
	std::string invalidCharacter;
	int lineNo;
public:
	InvalidCharacterException(std::string character, int line);

	const std::string getException(std::string character, int line);

	virtual ~InvalidCharacterException() throw() {}
};

#endif
