#ifndef RELATION_H
#define RELATION_H

#include <set>
#include <list>
#include <iostream>
#include "Tuple.h"
#include "Header.h"

class Relation {
private:
	std::set<Tuple> tuples;
	Header header;
public:
	Relation() {}

	Relation(std::vector<std::string> headerList)
	{
		addHeader(headerList);
	}

	Relation(std::list<std::string> headerList)
	{
		addHeader(headerList);
	}

	Relation(Header inHeader)
	{
		header = inHeader;
	}

	int getSize()
	{
		return tuples.size();
	}

	void addHeader(std::vector<std::string> inputList)
	{
		for (unsigned int i = 0; i < inputList.size(); i++)
		{
			header.addToBody(inputList.at(i));
		}
	}

	void addHeader(std::list<std::string> inputList)
	{
		for (auto item: inputList)
		{
			header.addToBody(item);
		}
	}

	void addHeader(std::string input)
	{
		header.addToBody(input);
	}

	Header getHeader()
	{
		return header;
	}

	void setHeader(Header inHeader)
	{
		header = inHeader;
	}

	std::set<Tuple> getTuples()
	{
		return tuples;
	}

	bool addTuple(Tuple tuple)
	{
		std::pair<std::set<Tuple>::iterator, bool> ret = tuples.insert(tuple);
		return ret.second;
	}

	Relation select(int index, std::string value)
	{
		Relation selectRel = Relation(header);
		for (auto item : tuples)
		{
			if (item.getVector().at(index) == value)
			{
				selectRel.addTuple(item);
			}
		}
		return selectRel;
	}

	Relation select(int index, int index2)
	{
		Relation selectRel = Relation(header);
		for (auto item : tuples)
		{
			if (item.getVector().at(index) == item.getVector().at(index2))
			{
				selectRel.addTuple(item);
			}
		}
		return selectRel;
	}

	Relation project(std::list<int> indexes)
	{
		Relation selectRel = Relation();
		for (auto index : indexes)
		{
			selectRel.addHeader(header.getVector().at(index));
		}
		for (auto item : tuples)
		{
			Tuple tuple = Tuple();
			for (auto index : indexes)
			{
				tuple.addToBody(item.getVector().at(index));
			}
			selectRel.addTuple(tuple);
		}
		return selectRel;
	}

	Relation rename(std::list<std::string> names)
	{
		Relation selectRel = Relation(names);
		for (auto item : tuples)
			selectRel.addTuple(item);
		return selectRel;
	}

	Relation join(const Relation& rel2)
	{
		//std::cout << "Made it into natural join" << std::endl;

		Relation relFinal = Relation();
		std::vector<int> colR1;
		std::vector<int> colR2;
		std::vector<int> colR2Diff;

		//combining relation headers
		//first relation headers
		for (int i = 0; i < header.getSize(); i++)
		{
			relFinal.addHeader(header.getVector().at(i));
		}
		//std::cout << "Header: " << relFinal.getHeader().toString() << std::endl;
		//second relation headers
		for (int i = 0; i < rel2.header.getSize(); i++)
		{
			bool isPresent = false;
			for (int j = 0; j < relFinal.getHeader().getSize(); j++)
			{
				if (relFinal.getHeader().getVector().at(j) == rel2.header.getVector().at(i))
				{
					isPresent = true;
					colR1.push_back(j);
					colR2.push_back(i);
				}
			}
			if (!isPresent)
			{
				relFinal.addHeader(rel2.header.getVector().at(i));
				colR2Diff.push_back(i);
			}
		}

		/*Unit test*/
		//std::cout << "Header: " << relFinal.getHeader().toString() << std::endl;

		//test each tuple and join them if possible
		if (colR2Diff.size() != rel2.header.getVector().size())
		{
			/*Unit test*/
			//std::cout << "Same Column R1: " << colR1.at(0) << std::endl;
			//std::cout << "Same Column R2: " << colR2.at(0) << std::endl;
			//std::cout << "Is joined" << std::endl;
			for (auto tupleR1 : tuples)
			{
				for (auto tupleR2 : rel2.tuples)
				{
					bool tupleJoinable = true;
					for (unsigned int i = 0; i < colR2.size(); i++)
					{
						if (tupleR1.getVector().at(colR1.at(i)) != tupleR2.getVector().at(colR2.at(i)))
						{
							tupleJoinable = false;
						}
					}
					if (tupleJoinable)
					{
						Tuple tupleFinal = tupleR1;
						for (int i = 0; i < tupleR2.getSize(); i++)
						{
							for (unsigned int j = 0; j < colR2Diff.size(); j++)
							{
								if (i == colR2Diff.at(j))
								{
									tupleFinal.addToBody(tupleR2.getVector().at(colR2Diff.at(j)));
								}
							}
						}
						relFinal.addTuple(tupleFinal);
					}
				}
			}
		}
		else
		{
			//std::cout << "Is unioned" << std::endl;
			for (auto tupleR1 : tuples)
			{
				for (auto tupleR2 : rel2.tuples)
				{
					Tuple tupleFinal = tupleR1;
					for (unsigned int j = 0; j < tupleR2.getVector().size(); j++)
					{

						tupleFinal.addToBody(tupleR2.getVector().at(j));
					}
					relFinal.addTuple(tupleFinal);
				}
			}
		}
		

		/*Unit Test*/
		//std::cout << "Made it through natural join" << std::endl;
		//std::cout << "Relation header" << relFinal.getHeader().toString() << std::endl;
		//relFinal.toString();
		return relFinal;
	}

	Relation uniqueTuples(const Relation& rel)
	{
		Relation relFinal = Relation(header);
		for (auto tuple : rel.tuples)
		{
			if (tuples.find(tuple) != tuples.end())
			{
				relFinal.addTuple(tuple);
			}
		}
		return relFinal;
	}
	
	void toString()
	{
		if (header.getSize() > 0)
		{
			for (std::set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++)
			{
				Tuple element = *i;
				std::cout << "  ";
				for (int j = 0; j < header.getSize(); j++)
				{
					if (j == 0)
					{
						std::cout << header.getVector().at(j) << "=" << element.getVector().at(j);
					}
					else
					{
						std::cout << ", " << header.getVector().at(j) << "=" << element.getVector().at(j);
					}
				}
				std::cout << std::endl;
			}
		}
	}
};
#endif /* RELATION_H */