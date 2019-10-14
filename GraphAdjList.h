#pragma once
#include"Graph.h"
#include<vector>

class GraphAdjList : public Graph
{

public:
	GraphAdjList(const size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
		adj_list_ = std::vector<std::vector<Vertex>>(vertex_count);
	}

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int weight = 0)
	{
		Graph::addEdge(start, finish);
		adj_list_[start].push_back(finish);
		if (!is_directed_)
		{
			adj_list_[finish].push_back(start);
		}
	}

	std::vector<Vertex> getNeighbors(const Vertex& v) const override
	{
		return adj_list_[v];
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		return adj_list_[v].size();
	}

	void printGraph() const
	{
		std::cout << "Print\n";
		std::cout << vertex_count_;
		for (Vertex i = 0; i < vertex_count_; i++)
		{
			for (Vertex k : adj_list_[i])
				std::cout << k << " ";
			std::cout << std::endl;
		}
	}

private:
	std::vector<std::vector<Vertex> adj_list_;
};
