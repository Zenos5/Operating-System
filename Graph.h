#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>
#include <vector>
#include <iostream>

class Graph {
private:
	std::map<int, std::set<int>> edges;
	std::map<int, bool> visited;
	std::vector<int> post_order;
	std::vector<std::vector<int>> scp;

	/*
	Procedure DepthFirstSearch(v: Vertex)
		mark v
		for each vertex w adjacent from v
			if w is not marked
				DepthFirstSearch(w)
	*/
	void dfs(int index)
	{
		visited[index] = true;
		for (auto edge : edges[index])
		{
			if (!visited[edge])
			{
				dfs(edge);
			}
		}
		post_order.push_back(index);
	}

	void dfs(int index, std::vector<int> searchOrder)
	{
		visited[index] = true;
		std::vector<int> sides;
		std::vector<int> order;
		for (auto edge : edges[index])
		{
			sides.push_back(edge);
		}

		for (unsigned int i = 0; i < searchOrder.size(); i++)
		{
			for (unsigned int j = 0; j < sides.size(); j++)
			{
				if (searchOrder.at(i) == sides.at(j))
				{
					order.push_back(searchOrder.at(i));
				}
			}
		}

		/*
		std::cout << "Order: ";
		for (unsigned int j = 0; j < sides.size(); j++)
		{
			std::cout << order.at(j) << " ";
		}
		std::cout << std::endl;
		*/
		
		for (unsigned int i = 0; i < order.size(); i++)
		{
			if (!visited[order.at(i)])
			{
				dfs(order.at(i));
			}
		}
		post_order.push_back(index);
	}
public:
	Graph() {}

	void addRuleToGraph(int initRule, std::set<int> follow)
	{
		edges.emplace(initRule, follow);
	}

	std::set<int> getSetAt(int name)
	{
		return edges[name];
	}

	void addToSet(int name, int data)
	{
		edges[name].emplace(data);
	}

	/*
	Procedure DepthFirstSearchForest(G: Graph)
		forest := empty
		for each vertex v in G
			clear the visit mark for v
		for each vertex v in G
			if v is not marked
				tree := DepthFirstSearch(v)
				add tree to forest
	*/
	void dfsForest()
	{
		visited.clear();
		std::vector<int> forest;
		for (unsigned int i = 0; i < edges.size(); i++)
		{
			visited.emplace(i, false);
		}

		for (unsigned int i = 0; i < edges.size(); i++)
		{
			std::vector<int> tree;
			if (!visited[i])
			{
				dfs(i);
				for (unsigned int x = 0; x < post_order.size(); x++)
					forest.push_back(post_order.at(x));
				post_order.clear();
			}
		}
		post_order = forest;
	}

	std::vector<std::set<int>> dfsForest(std::vector<int> postOrder)
	{
		visited.clear();
		std::vector<std::set<int>> forest;
		std::vector<int> searchOrder;
		for (unsigned int i = 0; i < postOrder.size(); i++)
		{
			searchOrder.insert(searchOrder.begin(), postOrder.at(i));
		}

		for (unsigned int i = 0; i < edges.size(); i++)
		{
			visited.emplace(i, false);
		}

		for (unsigned int i = 0; i < edges.size(); i++)
		{
			std::set<int> tree;
			if (!visited[searchOrder.at(i)])
			{
				dfs(searchOrder.at(i), searchOrder);
				for (unsigned int x = 0; x < post_order.size(); x++)
					tree.emplace(post_order.at(x));
				forest.push_back(tree);
				post_order.clear();
			}
		}
		return forest;
	}

	std::vector<int> getPostOrder()
	{
		return post_order;
	}
	
	void toString()
	{
		for (unsigned int i = 0; i < edges.size(); i++)
		{
			std::cout << "R" << i << ":";
			bool passed = false;
			for (auto edge : edges[i])
			{
				if (!passed)
				{
					std::cout << "R" << edge;
					passed = true;
				}
				else
				{
					std::cout << ",R" << edge;
				}
			}
			std::cout << std::endl;
		}
	}
};
#endif /* GRAPH_H */