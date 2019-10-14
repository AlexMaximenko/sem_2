#pragma once
#include<vector>
class WeightedGraph
{
public:
	typedef size_t Vertex;
	
	virtual int getEdgeWeight(const Vertex& from, const Vertex& to) = 0;


};