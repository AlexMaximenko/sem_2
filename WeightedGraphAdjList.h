#pragma once
#include"Graph.h"
#include"GraphAdjList.h"
#include"WeightedGraph.h"
#include<map>
class WeightedGraphAdjList : public GraphAdjList, public WeightedGraph
{
public:
	WeightedGraphAdjList(const size_t& vertex_count, const bool is_directed) :
		GraphAdjList(vertex_count, is_directed) {}
	
	void addEdge(const Vertex& start, const Vertex& finish, const int weight = 0)
	{
		GraphAdjList::addEdge(start, finish, weight);
		weights[std::make_pair(start, finish)] = weight;
	}

	int getEdgeWeight(const Vertex& from, const Vertex& to)
	{
		return weights[{from, to}];
	}

private:
	std::map<std::pair<Vertex, Vertex>, int> weights;
};