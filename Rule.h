#ifndef RULE_H
#define RULE_H

#include "Predicate.h"
#include <vector>

class Rule {
private:
	Predicate head;
	std::vector<Predicate> body;
public:
	Rule()
	{
		head = Predicate();
	}

	Rule(Predicate inputHead)
	{
		head = inputHead;
	}

	void addToBody(Predicate inputBody)
	{
		body.push_back(inputBody);
	}

	std::vector<Predicate> getPredicates() const
	{
		return body;
	}

	int getPredicateListSize() const
	{
		return body.size();
	}

	Predicate getHead() const
	{
		return head;
	}

	std::string toString() const
	{
		std::string output = head.toString() + " :- ";
		for (unsigned int i = 0; i < body.size(); i++)
		{
			if (i == 0)
			{
				output += body.at(i).toString();
			}
			else
			{
				output += "," + body.at(i).toString();
			}
		}
		output += ".";
		return output;
	}
};
#endif /* RULE_H */