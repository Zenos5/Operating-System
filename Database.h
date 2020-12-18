#ifndef DATABASE_H
#define DATABASE_H

#include <set>
#include <map>
#include <list>
#include <iostream>
#include <iterator>
#include "Relation.h"
#include "Predicate.h"
#include "Rule.h"
#include "Tuple.h"
#include "Graph.h"

class Database {
private:
	std::map<std::string, Relation> database;

	int rulePasses;

	void processSchemes(const std::vector<Predicate> schemeList)
	{
		for (unsigned int i = 0; i < schemeList.size(); i++)
		{
			database.emplace(schemeList.at(i).getName(), Relation(schemeList.at(i).getParameterList()));
		}
	}

	void processFacts(const std::vector<Predicate> factList)
	{
		for (unsigned int i = 0; i < factList.size(); i++)
		{
			Tuple tuple = Tuple();
			tuple.defineList(factList.at(i).getParameterList());
			database[factList.at(i).getName()].addTuple(tuple);
		}
	}

	Graph buildOriginalGraph(const std::vector<Rule> ruleList)
	{
		Graph orig = Graph();
		std::vector<std::string> ruleNames;
		for (unsigned int i = 0; i < ruleList.size(); i++)
		{
			ruleNames.push_back(ruleList.at(i).getHead().getName());
			std::set<int> follow;
			orig.addRuleToGraph(i, follow);
		}
		for (unsigned int i = 0; i < ruleNames.size(); i++)
		{
			for (unsigned int j = 0; j < ruleList.at(i).getPredicates().size(); j++)
			{
				for (unsigned int k = 0; k < ruleNames.size(); k++)
				{
					if (ruleNames.at(k) == ruleList.at(i).getPredicates().at(j).getName())
					{
						orig.addToSet(i, k);
					}
				}
			}
		}
		return orig;
	}

	Graph buildReverseGraph(const std::vector<Rule> ruleList)
	{
		Graph orig = Graph();
		std::vector<std::string> ruleNames;
		for (unsigned int i = 0; i < ruleList.size(); i++)
		{
			ruleNames.push_back(ruleList.at(i).getHead().getName());
			std::set<int> follow;
			orig.addRuleToGraph(i, follow);
		}
		for (unsigned int i = 0; i < ruleNames.size(); i++)
		{
			for (int j = 0; j < ruleList.at(i).getPredicateListSize(); j++)
			{
				for (unsigned int k = 0; k < ruleNames.size(); k++)
				{
					if (ruleNames.at(k) == ruleList.at(i).getPredicates().at(j).getName())
					{
						orig.addToSet(k, i);
					}
				}
			}
		}
		return orig;
	}

	void processRules(const std::vector<Rule> ruleList, std::vector<std::set<int>> scp)
	{
		//k = num of rules
		for (unsigned int k = 0; k < scp.size(); k++)
		{
			std::cout << "SCC: ";
			bool passed = false;
			for (auto item : scp.at(k))
			{
				if (!passed)
				{
					std::cout << "R" << item;
					passed = true;
				}
				else
				{
					std::cout << ",R" << item;
				}
			}
			std::cout << std::endl;
			
			std::vector<Rule> rules;
			for (auto item : scp.at(k))
			{
				rules.push_back(ruleList.at(item));
			}

			processSCP(rules);
			
			std::cout << rulePasses << " passes: ";
			passed = false;
			for (auto item : scp.at(k))
			{
				if (!passed)
				{
					std::cout << "R" << item;
					passed = true;
				}
				else
				{
					std::cout << ",R" << item;
				}
			}
			std::cout << std::endl;

			rulePasses = 0;
		}
	}

	void processSCP(std::vector<Rule> ruleList)
	{
		bool reprocess = false;
		//k = num of rules
		for (unsigned int k = 0; k < ruleList.size(); k++)
		{
			std::vector<Relation> relations;
			/*Unit test*/
			//std::cout << "Rule " << k << std::endl;
			//i = num of following predicates
			for (unsigned int i = 0; i < ruleList.at(k).getPredicates().size(); i++)
			{
				std::list<int> indexes;
				std::list<std::string> names;
				std::set<std::string> headers;
				Relation r = database[ruleList.at(k).getPredicates().at(i).getName()];
				//j = num of parameters
				for (unsigned int j = 0; j < ruleList.at(k).getPredicates().at(i).getParameterList().size(); j++)
				{
					//element
					if (ruleList.at(k).getPredicates().at(i).getParameterList().at(j).at(0) == '\'')
					{
						r = r.select(j, ruleList.at(k).getPredicates().at(i).getParameterList().at(j));
					}
					//variable
					if (ruleList.at(k).getPredicates().at(i).getParameterList().at(j).at(0) != '\'')
					{
						//shared variable
						if (headers.find(ruleList.at(k).getPredicates().at(i).getParameterList().at(j)) != headers.end())
						{
							int dex = 0;
							for (unsigned int k = 0; k < ruleList.at(k).getPredicates().at(i).getParameterList().size(); k++)
							{
								if (ruleList.at(k).getPredicates().at(i).getParameterList().at(j) == ruleList.at(k).getPredicates().at(i).getParameterList().at(k) && k < j)
								{
									dex = (int)k;
								}
							}
							r = r.select(dex, j);
						}
						//unique variable
						else
						{
							indexes.push_back(j);
							names.push_back(ruleList.at(k).getPredicates().at(i).getParameterList().at(j));
							headers.insert(ruleList.at(k).getPredicates().at(i).getParameterList().at(j));
						}
					}
				}
				r = r.project(indexes);
				r = r.rename(names);

				relations.push_back(r);

				/*Unit Test*/
				//std::cout << "Relation " << i << std::endl;
				//r.toString();

				/*Unit test*/
				//std::cout << "Predicate Loop " << x << std::endl;
			}
			//std::cout << "Exit Predicate Loop" << std::endl;


			Relation finalRel = relations.at(0);
			//natural join predicates
			if (relations.size() > 1)
			{
				for (unsigned int i = 0; i < relations.size(); i++)
				{
					if (i > 0)
					{
						finalRel = finalRel.join(relations.at(i));
					}
					/*Unit test*/
					//std::cout << "Join Loop " << i << std::endl;
				}
			}
			/*Unit test*/
			//std::cout << "Made out of join Loop " << std::endl;
			//finalRel.toString();

			std::list<int> indexes;
			for (unsigned int j = 0; j < ruleList.at(k).getHead().getParameterList().size(); j++)
			{
				for (unsigned int i = 0; i < finalRel.getHeader().getVector().size(); i++)
				{
					if (finalRel.getHeader().getVector().at(i) == ruleList.at(k).getHead().getParameterList().at(j))
					{
						indexes.push_back(i);
					}
				}
			}
			finalRel = finalRel.project(indexes);
			std::list<std::string> headParameterList;
			for (unsigned int i = 0; i < ruleList.at(k).getHead().getParameterList().size(); i++)
			{
				headParameterList.push_back(ruleList.at(k).getHead().getParameterList().at(i));
			}
			finalRel = finalRel.rename(headParameterList);

			Relation outputRel = Relation(database[ruleList.at(k).getHead().getName()].getHeader());

			//union
			bool check = false;
			for (auto tuple : finalRel.getTuples())
			{
				check = database[ruleList.at(k).getHead().getName()].addTuple(tuple);
				if (check)
				{
					outputRel.addTuple(tuple);
					reprocess = true;
				}
			}

			//output
			std::cout << ruleList.at(k).toString() << std::endl;
			//finalRel.toString();
			outputRel.toString();

			/*Unit test*/
			//std::cout << "Rule Loop " << k << std::endl;
		}

		rulePasses++;

		if (ruleList.size() == 1)
		{
			bool dependantOnSelf = false;
			for (unsigned int j = 0; j < ruleList.at(0).getPredicates().size(); j++)
			{
				if (ruleList.at(0).getHead().getName() == ruleList.at(0).getPredicates().at(j).getName())
				{
					dependantOnSelf = true;
					//std::cout << "self loop" << std::endl;
				}
			}

			if (!dependantOnSelf)
			{
				reprocess = false;
			}
			//std::cout << "Size is 1" << std::endl;
		}

		if (reprocess)
		{
			/*Unit test*/
			//std::cout << "Re Process Rules" << std::endl;
			processSCP(ruleList);
		}
	}

	void processQueries(const std::vector<Predicate> queryList)
	{
		std::cout << "Query Evaluation" << std::endl;
		for (unsigned int i = 0; i < queryList.size(); i++)
		{
			std::list<int> indexes;
			std::list<std::string> names;
			std::set<std::string> headers;
			Relation r = database[queryList.at(i).getName()];
			for (unsigned int j = 0; j < queryList.at(i).getParameterList().size(); j++)
			{
				//element
				if (queryList.at(i).getParameterList().at(j).at(0) == '\'')
				{
					r = r.select(j, queryList.at(i).getParameterList().at(j));
				}
				//variable
				if (queryList.at(i).getParameterList().at(j).at(0) != '\'')
				{
					//shared variable
					if (headers.find(queryList.at(i).getParameterList().at(j)) != headers.end())
					{
						int dex = 0;
						for (unsigned int k = 0; k < queryList.at(i).getParameterList().size(); k++)
						{
							if (queryList.at(i).getParameterList().at(j) == queryList.at(i).getParameterList().at(k) && k < j)
							{
								dex = (int)k;
							}
						}
						r = r.select(dex, j);
					}
					//unique variable
					else
					{
						indexes.push_back(j);
						names.push_back(queryList.at(i).getParameterList().at(j));
						headers.insert(queryList.at(i).getParameterList().at(j));
					}
				}
			}
			r = r.project(indexes);
			r = r.rename(names);
			std::cout << queryList.at(i).toString() << "? ";
			if (r.getSize() > 0 && r.getSize() <= database[queryList.at(i).getName()].getSize())
			{
				std::cout << "Yes(" << r.getSize() << ")" << std::endl;
				r.toString();
			}
			else
			{
				std::cout << "No" << std::endl;
			}
		}
	}
public:
	Database() 
	{
		rulePasses = 0;
	}

	void process(const std::vector<Predicate> schemeList, const std::vector<Predicate> factList,
		const std::vector<Rule> ruleList, const std::vector<Predicate> queryList)
	{
		//scheme setup
		processSchemes(schemeList);

		//facts added in
		processFacts(factList);

		
		//graph building
		Graph originalGraph = buildOriginalGraph(ruleList);
		Graph reverseGraph = buildReverseGraph(ruleList);
		std::cout << "Dependency Graph" << std::endl;
		originalGraph.toString();
		std::cout << std::endl;

		reverseGraph.dfsForest();

		std::vector<std::set<int>> scp = originalGraph.dfsForest(reverseGraph.getPostOrder());
		

		//rules evaluated
		std::cout << "Rule Evaluation" << std::endl;
		processRules(ruleList, scp);
		std::cout << std::endl;

		//queries given
		processQueries(queryList);
	}
};
#endif /* DATABASE_H */