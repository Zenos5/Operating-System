#ifndef DATALOG_PROGRAM_H
#define DATALOG_PROGRAM_H

#include "Predicate.h"
#include "Rule.h"
#include "Token.h"
#include <vector>
#include <set>
#include <string>

class DatalogProgram {
private:
	int index;
	bool notError;
	Token error;
	std::vector<Predicate> schemes;
	std::vector<Predicate> facts;
	std::vector<Predicate> queries;
	std::vector<Rule> rules;
	std::set<std::string> domain;
	std::vector<Token> tokenList;

	//Top Level
	void parseDatalogProgram()
	{
		//Schemes
		if (tokenList.at(index).getType() == SCHEMES)
		{
			index++;
			if (tokenList.at(index).getType() == COLON)
			{
				index++;
				parseScheme();
				parseSchemeList();
				index++;

				//Facts
				if (tokenList.at(index).getType() == COLON)
				{
					index++;
					parseFactList();
					index++;

					//Rules
					if (tokenList.at(index).getType() == COLON)
					{
						index++;
						parseRuleList();
						index++;

						//Queries
						if (tokenList.at(index).getType() == COLON)
						{
							index++;
							parseQuery();
							parseQueryList();
							index++;

						}
						else
						{
							throw tokenList.at(index);
						}
					}
					else
					{
						throw tokenList.at(index);
					}
				}
				else
				{
					throw tokenList.at(index);
				}
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	//Types
	void parseScheme()
	{
		Predicate scheme;
		if (tokenList.at(index).getType() == ID)
		{
			scheme = Predicate(tokenList.at(index).getContent());
			index++;
			if (tokenList.at(index).getType() == LEFT_PAREN)
			{
				index++;
				if (tokenList.at(index).getType() == ID)
				{
					scheme.addParameter(Parameter(tokenList.at(index).getContent()));
					index++;
					parseIdList(scheme);
					if (tokenList.at(index).getType() == RIGHT_PAREN)
					{
						schemes.push_back(scheme);
						index++;
					}
					else
					{
						throw tokenList.at(index);
					}
				}
				else
				{
					throw tokenList.at(index);
				}
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	void parseFact()
	{
		Predicate fact;
		if (tokenList.at(index).getType() == ID)
		{
			fact = Predicate(tokenList.at(index).getContent());
			index++;
			if (tokenList.at(index).getType() == LEFT_PAREN)
			{
				index++;
				if (tokenList.at(index).getType() == STRING)
				{
					fact.addParameter(Parameter(tokenList.at(index).getContent()));
					domain.insert(tokenList.at(index).getContent());
					index++;
					parseStringList(fact);
					if (tokenList.at(index).getType() == RIGHT_PAREN)
					{
						index++;
						if (tokenList.at(index).getType() == PERIOD)
						{
							facts.push_back(fact);
							index++;
						}
						else
						{
							throw tokenList.at(index);
						}
					}
					else
					{
						throw tokenList.at(index);
					}
				}
				else
				{
					throw tokenList.at(index);
				}
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	void parseRule()
	{
		Rule rule;
		parseHeadPredicate(rule);
		if (tokenList.at(index).getType() == COLON_DASH)
		{
			index++;
			parsePredicate(rule);
			parsePredicateList(rule);
			if (tokenList.at(index).getType() == PERIOD)
			{
				rules.push_back(rule);
				index++;
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	void parseQuery()
	{
		Predicate query;
		parsePredicate(query);
		if (tokenList.at(index).getType() == Q_MARK)
		{
			queries.push_back(query);
			index++;
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	//Type Lists
	void parseSchemeList()
	{
		if (tokenList.at(index).getType() != FACTS && tokenList.at(index).getType() != EOF)
		{
			parseScheme();
			parseSchemeList();
		}
		if (tokenList.at(index).getType() == EOF)
		{
			throw tokenList.at(index);
		}
	}

	void parseFactList()
	{
		if (tokenList.at(index).getType() != RULES && tokenList.at(index).getType() != EOF)
		{
			parseFact();
			parseFactList();
		}
		if (tokenList.at(index).getType() == EOF)
		{
			throw tokenList.at(index);
		}
	}

	void parseRuleList()
	{
		if (tokenList.at(index).getType() != QUERIES && tokenList.at(index).getType() != EOF)
		{
			parseRule();
			parseRuleList();
		}
		if (tokenList.at(index).getType() == EOF)
		{
			throw tokenList.at(index);
		}
	}

	void parseQueryList()
	{
		if (tokenList.at(index).getType() == ID)
		{
			parseQuery();
			parseQueryList();
		}
	}

	//Rule Setup
	void parseHeadPredicate(Rule& first)
	{
		Predicate head;
		if (tokenList.at(index).getType() == ID)
		{
			head = Predicate(tokenList.at(index).getContent());
			index++;
			if (tokenList.at(index).getType() == LEFT_PAREN)
			{
				index++;
				if (tokenList.at(index).getType() == ID)
				{
					head.addParameter(Parameter(tokenList.at(index).getContent()));
					index++;
					parseIdList(head);
					if (tokenList.at(index).getType() == RIGHT_PAREN)
					{
						first = Rule(head);
						index++;
					}
					else
					{
						throw tokenList.at(index);
					}
				}
				else
				{
					throw tokenList.at(index);
				}
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	void parsePredicate(Rule& input)
	{
		Predicate predicate;
		if (tokenList.at(index).getType() == ID)
		{
			predicate = Predicate(tokenList.at(index).getContent());
			index++;
			if (tokenList.at(index).getType() == LEFT_PAREN)
			{
				index++;
				predicate.addParameter(Parameter(parseParameter()));
				index++;
				parseParameterList(predicate);
				if (tokenList.at(index).getType() == RIGHT_PAREN)
				{
					input.addToBody(predicate);
					index++;
				}
				else
				{
					throw tokenList.at(index);
				}
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	void parsePredicate(Predicate& input)
	{
		if (tokenList.at(index).getType() == ID)
		{
			input = Predicate(tokenList.at(index).getContent());
			index++;
			if (tokenList.at(index).getType() == LEFT_PAREN)
			{
				index++;
				input.addParameter(Parameter(parseParameter()));
				index++;
				parseParameterList(input);
				if (tokenList.at(index).getType() == RIGHT_PAREN)
				{
					index++;
				}
				else
				{
					throw tokenList.at(index);
				}
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	//Data Lists
	void parsePredicateList(Predicate& input)
	{
		if (tokenList.at(index).getType() == COMMA)
		{
			index++;
			parsePredicate(input);
			index++;
			parsePredicateList(input);
		}
	}

	//Data Lists
	void parsePredicateList(Rule& input)
	{
		if (tokenList.at(index).getType() == COMMA)
		{
			index++;
			parsePredicate(input);
			parsePredicateList(input);
		}
	}

	void parseParameterList(Predicate& input)
	{
		if (tokenList.at(index).getType() == COMMA)
		{
			index++;
			input.addParameter(Parameter(parseParameter()));
			index++;
			parseParameterList(input);
		}
	}

	void parseStringList(Predicate& input)
	{
		if (tokenList.at(index).getType() == COMMA)
		{
			index++;
			if (tokenList.at(index).getType() == STRING)
			{
				input.addParameter(Parameter(tokenList.at(index).getContent()));
				domain.insert(tokenList.at(index).getContent());
				index++;
				parseStringList(input);
			}
			else
			{
				throw (tokenList.at(index));
			}
		}
	}

	void parseIdList(Predicate& input)
	{
		if (tokenList.at(index).getType() == COMMA)
		{
			index++;
			if (tokenList.at(index).getType() == ID)
			{
				input.addParameter(tokenList.at(index).getContent());
				index++;
				parseIdList(input);
			}
			else
			{
				throw (tokenList.at(index));
			}
		}
	}

	//Data
	std::string parseParameter()
	{
		if (tokenList.at(index).getType() == ID)
		{
			return tokenList.at(index).getContent();
		}
		else if (tokenList.at(index).getType() == STRING)
		{
			return tokenList.at(index).getContent();
		}
		else
		{
			return parseExpression();
		}
	}

	std::string parseExpression()
	{
		std::string expression = "";
		if (tokenList.at(index).getType() == LEFT_PAREN)
		{
			index++;
			expression += "(" + parseParameter();
			index++;
			expression += parseOperator();
			index++;
			expression += parseParameter() + ")";
			index++;
			if (tokenList.at(index).getType() == RIGHT_PAREN)
			{
				return expression;
			}
			else
			{
				throw tokenList.at(index);
			}
		}
		else
		{
			throw tokenList.at(index);
		}
	}

	std::string parseOperator()
	{
		if (tokenList.at(index).getType() == ADD)
		{
			return tokenList.at(index).getContent();
		}
		else if (tokenList.at(index).getType() == MULTIPLY)
		{
			return tokenList.at(index).getContent();
		}
		else
		{
			throw tokenList.at(index);
		}
	}

public:
	DatalogProgram()
	{
		notError = true;
		index = 0;
	}

	void inputTokenList(std::vector<Token> list)
	{
		tokenList = list;
	}

	//Starting Parse
	void parse()
	{
		try {
			parseDatalogProgram();
		}
		catch (Token issue)
		{
			error = issue;
			notError = false;
		}	
	}

	std::vector<Predicate> getSchemes()
	{
		std::vector<Predicate> newVec = schemes;
		return newVec;
	}

	std::vector<Predicate> getFacts()
	{
		std::vector<Predicate> newVec = facts;
		return newVec;
	}

	std::vector<Predicate> getQueries()
	{
		std::vector<Predicate> newVec = queries;
		return newVec;
	}

	std::vector<Rule> getRules()
	{
		std::vector<Rule> newVec = rules;
		return newVec;
	}

	//To String
	std::string toString()
	{
		std::string output = "";
		if (notError)
		{
			output += "Success!\n";

			output += "Schemes(" + std::to_string(schemes.size()) + "):\n";
			for (unsigned int i = 0; i < schemes.size(); i++)
			{
				output += "  " + schemes.at(i).toString() + "\n";
			}

			output += "Facts(" + std::to_string(facts.size()) + "):\n";
			for (unsigned int i = 0; i < facts.size(); i++)
			{
				output += "  " + facts.at(i).toString() + ".\n";
			}

			output += "Rules(" + std::to_string(rules.size()) + "):\n";
			for (unsigned int i = 0; i < rules.size(); i++)
			{
				output += "  " + rules.at(i).toString() + "\n";
			}

			output += "Queries(" + std::to_string(queries.size()) + "):\n";
			for (unsigned int i = 0; i < queries.size(); i++)
			{
				output += "  " + queries.at(i).toString() + "?\n";
			}

			output += "Domain(" + std::to_string(domain.size()) + "):\n";
			for (auto item : domain)
				output += "  " + item + "\n";
		}
		else
		{
			output += "Failure!\n";
			output += "  " + error.toString() + "\n";
		}
		return output;
	}
};
#endif /* DATALOG_PROGRAM_H */