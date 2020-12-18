#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include "Tokenizer.h"
#include <vector>
#include <sstream>

class Lexer {
private:
	std::vector<Token> tokenList;
	Tokenizer processor;
public:
	Lexer()
	{
		processor = Tokenizer();
	}

	void nextPeek(char symbol)
	{
		processor.nextPeek(symbol);
		Token currentToken = processor.getNextToken();
		if (currentToken.returnType() != "ERROR")
		{
			tokenList.push_back(currentToken);
		}
	}

	void nextChar(char symbol)
	{
		processor.nextChar(symbol);
	}

	void finalToken()
	{
		tokenList.push_back(Token(END_OF_FILE, processor.getLineNumber(), ""));
	}

	std::string outputTokenList()
	{
		std::string output = "";
		for (unsigned int i = 0; i < tokenList.size(); i++)
		{
			output = output + tokenList.at(i).toString() + "\n";
		}
		return output;
	}

	std::vector<Token> outputList()
	{
		return tokenList;
	}

	std::string tokenNum()
	{
		std::stringstream transfer;
		transfer << tokenList.size();
		std::string value;
		transfer >> value;
		return value;
	}
};
#endif /* TOKENIZER_H */