#ifndef HEADER_H
#define HEADER_H

#include <vector>

class Header {
private:
	std::vector<std::string> names;
public:
	Header() {}

	Header(std::vector<std::string> input)
	{
		names = input;
	}

	void addToBody(std::string input)
	{
		names.push_back(input);
	}

	std::vector<std::string> getVector() const
	{
		return names;
	}

	int getSize() const
	{
		return names.size();
	}

	std::string toString()
	{
		std::string output = "";
		for (unsigned int i = 0; i < names.size(); i++)
		{
			if (i == 0)
			{
				output += names.at(i);
			}
			else
			{
				output += "," + names.at(i);
			}
		}
		return output;
	}
};
#endif /* HEADER_H */