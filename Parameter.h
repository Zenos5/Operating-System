#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

class Parameter {
private:
	std::string content;
public:
	Parameter()
	{
		content = "";
	}

	Parameter(std::string input)
	{
		content = input;
	}

	std::string toString() const
	{
		return content;
	}
};
#endif /* PARAMETER_H */