#pragma once
#include"Graph.h"
#include<vector>

class GraphAdjMatrix : public Graph
{
public:
	GraphAdjMatrix(const size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
		adj_matrix_ = std::vector<std::vector<int>>(vertex_count, std::vector<int>(vertex_count, 0));
	}

	void addEdge(const Vertex& start, const Vertex& finish, const int weight = 0) override
	{
		Graph::addEdge(start, finish);
		adj_matrix_[start][finish] = 1;
		if (!is_directed_)
		{
			adj_matrix_[finish][start] = 1;
		}
	}

	std::vector<Vertex> getNeighbors(const Vertex& v) const override
	{
		std::vector<Vertex> neighbors;
		for (Vertex neighbor = 0; neighbor < vertex_count_; neighbor++)
		{
			if (adj_matrix_[v][neighbor])
			{
				neighbors.push_back(neighbor);
			}
		}
		return neighbors;
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		size_t count = 0;
		for (Vertex u = 0; u < vertex_count_; u++)
		{
			if (adj_matrix_[v][u])
			{
				count++;
			}
		}
		return count;
	}

private:
	std::vector<std::vector<int>> adj_matrix_;
};