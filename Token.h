#ifndef TOKEN_H
#define TOKEN_H

enum type {STRING, SCHEMES, COLON, ID, LEFT_PAREN, RIGHT_PAREN, COMMA, FACTS, PERIOD, RULES, COLON_DASH, QUERIES, Q_MARK, COMMENT, UNDEFINED, END_OF_FILE, FILLER, MULTIPLY, ADD};

class Token {
private:
	type tokenType;
	int lineNumber;
	std::string tokenContent;
public:
	Token()
	{
		tokenType = UNDEFINED;
		lineNumber = -1;
		tokenContent = "";
	}

	Token(type inputType, int lineNum, std::string content)
	{
		tokenType = inputType;
		lineNumber = lineNum;
		tokenContent = content;
	}

	std::string returnType()
	{
		std::string value = "";
		switch (tokenType) {
			case STRING: return "STRING"; break;
			case SCHEMES: return "SCHEMES"; break;
			case COLON: return "COLON"; break;
			case COLON_DASH: return "COLON_DASH"; break;
			case ID: return "ID"; break;
			case QUERIES: return "QUERIES"; break;
			case LEFT_PAREN: return "LEFT_PAREN"; break;
			case RIGHT_PAREN: return "RIGHT_PAREN"; break;
			case COMMA: return "COMMA"; break;
			case FACTS: return "FACTS"; break;
			case PERIOD: return "PERIOD"; break;
			case RULES:return "RULES"; break;
			case Q_MARK: return "Q_MARK"; break;
			case COMMENT: return "COMMENT"; break;
			case UNDEFINED: return "UNDEFINED"; break;
			case END_OF_FILE: return "EOF"; break;
			case MULTIPLY: return "MULTIPLY"; break;
			case ADD: return "ADD"; break;
			default: return "ERROR";
		}
		return "ERROR";
	}

	type getType()
	{
		return tokenType;
	}

	std::string getContent()
	{
		return tokenContent;
	}

	std::string toString()
	{
		return "(" + returnType() + "," + "\"" + tokenContent + "\"" + "," + std::to_string(lineNumber) + ")";
	}
};
#endif /* TOKEN_H */