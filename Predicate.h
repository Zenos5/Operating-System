#ifndef PREDICATE_H
#define PREDICATE_H

#include "Parameter.h"
#include <vector>
#include <string>

class Predicate {
private:
	std::string name;
	std::vector<Parameter> parameters;
public:
	Predicate()
	{
		name = "";
	}

	Predicate(std::string inputString)
	{
		name = inputString;
	}

	void addParameter(Parameter parameter)
	{
		parameters.push_back(parameter);
	}

	std::string getName() const
	{
		std::string givenName = name;
		return givenName;
	}

	std::vector<std::string> getParameterList() const
	{
		std::vector<std::string> outputList;
		for (unsigned int i = 0; i < parameters.size(); i++)
		{
			outputList.push_back(parameters.at(i).toString());
		}
		return outputList;
	}

	std::string toString() const
	{
		std::string output = name + "(";
		for (unsigned int i = 0; i < parameters.size(); i++)
		{
			if (i == 0)
			{
				output += parameters.at(i).toString();
			}
			else
			{
				output += "," + parameters.at(i).toString();
			}
		}
		output += ")";
		return output;
	}
};
#endif /* PREDICATE_H */