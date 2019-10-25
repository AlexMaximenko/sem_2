#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<queue>
class Graph
{
public:
	typedef size_t Vertex;
	Graph(size_t vertex_count, bool is_directed)
	{
		vertex_count_ = vertex_count;
		is_directed_ = is_directed;
	}

	size_t getVertexCount() const
	{
		return vertex_count_;
	}

	size_t geEdgeCount() const
	{
		return edge_count_;
	}

	virtual std::vector<Vertex> getNeighbors(const Vertex& v) const = 0;

	virtual void addEdge(const Vertex& start, const Vertex& finish)
	{
		edge_count_++;
	}

	virtual size_t getNeighborsCount(const Vertex& v) const = 0;

protected:
	bool is_directed_;
	size_t vertex_count_,
		edge_count_;
};

class GraphAdjList : public Graph
{

public:
	GraphAdjList(const size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
		adj_list_ = std::vector<std::vector<Vertex>>(vertex_count);
	}

	void addEdge(const Vertex& start, const Vertex& finish)
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
private:
	std::vector<std::vector<Vertex>> adj_list_;
};

namespace GraphProcessing
{

	typedef size_t Vertex;
	enum VertexMark
	{
		WHITE, BLACK, GREY
	};
	const size_t WASNT_VISIT = -1;
	const size_t INF = -1;

	void addNeighbors(Graph& g, const size_t& vertex)
	{
		std::vector<size_t> neighbors;
		size_t a_1 = vertex / 1000;
		size_t a_2 = (vertex % 1000) / 100;
		size_t a_3 = (vertex % 100) / 10;
		size_t a_4 = vertex % 10;
		if (a_1 != 9)
		{
			g.addEdge(vertex, vertex + 1000);
		}
		if (a_4 != 1)
		{
			g.addEdge(vertex, vertex - 1);
		}
		g.addEdge(vertex, a_2 * 1000 + a_3 * 100 + a_4 * 10 + a_1);
		g.addEdge(vertex, a_4 * 1000 + a_1 * 100 + a_2 * 10 + a_3);
	}

	std::vector<Graph::Vertex> getWay(const std::vector<Graph::Vertex>& prev, const Graph::Vertex& finish)
	{
		std::vector<Graph::Vertex> way;
		Graph::Vertex curr = finish;
		while (prev[curr] != WASNT_VISIT)
		{
			way.push_back(curr);
			curr = prev[curr];
		}
		way.push_back(curr);
		std::reverse(way.begin(), way.end());
		return way;
	}
	std::vector<size_t> getShortestCombination(Graph& g, const Graph::Vertex& start, const Graph::Vertex& finish)
	{
		std::vector<Graph::Vertex> prev(g.getVertexCount(), WASNT_VISIT);
		std::vector<size_t> distance(g.getVertexCount(), INF);
		distance[start] = 0;
		std::queue<Graph::Vertex> qu;
		qu.push(start);
		while (distance[finish] == WASNT_VISIT)
		{
			Graph::Vertex curr = qu.front();
			qu.pop();
			addNeighbors(g, curr);
			for (Graph::Vertex neighbor : g.getNeighbors(curr))
			{
				if (distance[neighbor] == WASNT_VISIT)
				{
					prev[neighbor] = curr;
					distance[neighbor] = distance[curr] + 1;
					qu.push(neighbor);
				}
			}
		}
		return getWay(prev, finish);
	}
}


int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(10000, true);
	GraphProcessing::addNeighbors(g, n);
	std::vector<Graph::Vertex> combination = GraphProcessing::getShortestCombination(g, n, m);
	for (Graph::Vertex v : combination)
	{
		std::cout << v << std::endl;
	}
	return 0;
}