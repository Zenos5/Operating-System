#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Token.h"
#include <iostream>

class Tokenizer {
private:
	std::string input;
	char peekVal;
	int lineNumber;
	int start;
	bool lineComment;
	int counter;
public:
	Tokenizer()
	{
		input = "";
		lineComment = false;
		counter = 0;
		lineNumber = 1;
		start = 1;
	}

	int getLineNumber()
	{
		return lineNumber;
	}

	bool inputRemaining()
	{
		bool remaining = true;
		if (input == "")
		{
			remaining = false;
		}
		return remaining;
	}

	void nextChar(char value)
	{
		input += value;
	}

	void nextPeek(char value)
	{
		peekVal = value;
	}

	Token getNextToken()
	{
		Token currentToken = Token(FILLER, lineNumber, input);

		switch (input.at(0)) {
		//COLON, COLON_DASH
		case ':':
			if ((peekVal != '-') && (input.size() == 1))
			{
				currentToken = Token(COLON, lineNumber, input);
				input = "";
			}
			if (input.size() == 2)
			{
				currentToken = Token(COLON_DASH, lineNumber, input);
				input = "";
			}
			break;

		//COMMA
		case ',':
			currentToken = Token(COMMA, lineNumber, input);
			input = "";
			break;

		//PERIOD
		case '.':
			currentToken = Token(PERIOD, lineNumber, input);
			input = "";
			break;

		//LEFT_PAREN
		case '(':
			currentToken = Token(LEFT_PAREN, lineNumber, input);
			input = "";
			break;

		//RIGHT_PAREN
		case ')':
			currentToken = Token(RIGHT_PAREN, lineNumber, input);
			input = "";
			break;

		//Q_MARK
		case '?':
			currentToken = Token(Q_MARK, lineNumber, input);
			input = "";
			break;

		/*For Fun*/
		//MULTIPLY
		case '*':
			currentToken = Token(MULTIPLY, lineNumber, input);
			input = "";
			break;
		//ADD
		case '+':
			currentToken = Token(ADD, lineNumber, input);
			input = "";
			break;

		//ID, QUERIES
		//lowercase letters
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		//uppercase letters
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			if (!isalnum(peekVal))
			{
				if (input == "Queries")
				{
					currentToken = Token(QUERIES, lineNumber, input);
					input = "";
				}
				else if (input == "Schemes")
				{
					currentToken = Token(SCHEMES, lineNumber, input);
					input = "";
				}
				else if (input == "Rules")
				{
					currentToken = Token(RULES, lineNumber, input);
					input = "";
				}
				else if (input == "Facts")
				{
					currentToken = Token(FACTS, lineNumber, input);
					input = "";
				}
				else
				{
					currentToken = Token(ID, lineNumber, input);
					input = "";
				}
			}
			break;

		//STRING
		case '\'':
			if (input.size() == 1)
			{
				start = lineNumber;
				counter = 1;
			}
			if (input.size() > 1)
			{
				if (input.at(input.size() - 1) == '\n')
				{
					lineNumber++;
				}
				if (input.at(input.size() - 1) == '\'')
				{
					counter++;
					if (counter == 2 && peekVal != '\'')
					{
						currentToken = Token(STRING, start, input);
						input = "";
					}
					else if (counter % 2 == 0 && peekVal != '\'')
					{
						currentToken = Token(STRING, start, input);
						input = "";
					}
				}
				if (peekVal == EOF)
				{
					currentToken = Token(UNDEFINED, start, input);
					input = "";
				}
			}
			break;

		//COMMENT
		case '#':
			if (input.size() == 1)
			{
				start = lineNumber;
				if (peekVal != '|')
				{
					lineComment = true;
				}
			}
			if (input.size() > 1 && lineComment)
			{
				if (peekVal == '\n')
				{
					//currentToken = Token(COMMENT, start, input);
					input = "";
					lineComment = false;
				}
			}
			if (input.size() > 1 && !lineComment)
			{
				if (input.at(input.size() - 1) == '\n')
				{
					lineNumber++;
				}
				if (input.size() > 3 && input.at(input.size() - 1) == '#' && input.at(input.size() - 2) == '|')
				{
					//currentToken = Token(COMMENT, start, input);
					input = "";
					lineComment = false;
				}
				if (peekVal == EOF)
				{
					currentToken = Token(UNDEFINED, start, input);
					input = "";
				}
			}
			break;

		//Spaces
		case ' ':
		case '\t':
			input = "";
			break;
		case '\n':
			input = "";
			lineNumber++;
			break;

		default:
			currentToken = Token(UNDEFINED, lineNumber, input);
			input = "";
		}

		return currentToken;
	}
};#endif /* TOKENIZER_H */