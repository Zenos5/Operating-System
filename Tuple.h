#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <string>

class Tuple {
private:
	std::vector<std::string> values;
public:
	Tuple() {}

	bool operator< (const Tuple& other) const {
		std::string tuple1 = "";
		std::string tuple2 = "";
		for (unsigned int i = 0; i < values.size(); i++)
		{
			tuple1 += values.at(i);
		}
		for (unsigned int i = 0; i < values.size(); i++)
		{
			tuple2 += other.values.at(i);
		}

		unsigned int index = 0;
		while (tuple1.size() > index && tuple2.size() > index)
		{
			if (toupper(tuple2.at(index)) > toupper(tuple1.at(index)))
			{
				return true;
			}
			if (toupper(tuple2.at(index)) < toupper(tuple1.at(index)))
			{
				return false;
			}
			index++;
		}
		return false;
	}

	void addToBody(std::string input)
	{
		values.push_back(input);
	}
	
	void defineList(std::vector<std::string> inputList)
	{
		values = inputList;
	}

	std::vector<std::string> getVector()
	{
		return values;
	}

	int getSize() const
	{
		return values.size();
	}

	std::string toString()
	{
		std::string output = "";
		for (unsigned int i = 0; i < values.size(); i++)
		{
			if (i == 0)
			{
				output += values.at(i);
			}
			else
			{
				output += "," + values.at(i);
			}
		}
		return output;
	}
};
#endif /* TUPLE_H */