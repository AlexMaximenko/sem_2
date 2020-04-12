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

namespace GamePocessing
{
	std::vector<size_t> getPossibleNumbers(size_t current_number)
	{
		std::vector<size_t> possible_numbers;
		size_t a_1 = current_number / 1000;
		size_t a_2 = (current_number % 1000) / 100;
		size_t a_3 = (current_number % 100) / 10;
		size_t a_4 = current_number % 10;
		if (a_1 != 9)
		{
			possible_numbers.push_back(current_number + 1000);
		}
		if (a_4 != 1)
		{
			possible_numbers.push_back(current_number - 1);
		}
		possible_numbers.push_back(a_2 * 1000 + a_3 * 100 + a_4 * 10 + a_1);
		possible_numbers.push_back(a_4 * 1000 + a_1 * 100 + a_2 * 10 + a_3);
		return possible_numbers;
	}
}
namespace GraphProcessing
{
	namespace
	{
		const size_t WASNT_VISIT = -1;
		const size_t INF = -1;

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
	}
	std::vector<size_t> getShortestWay(Graph& g, const Graph::Vertex& start, const Graph::Vertex& finish)
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
	for (size_t i = 1000; i < 10000; i++)
	{
		for (Graph::Vertex j : GamePocessing::getPossibleNumbers(i))
		{
			g.addEdge(i, j);
		}
	}
	std::vector<Graph::Vertex> combination = GraphProcessing::getShortestWay(g, n, m);
	for (Graph::Vertex v : combination)
	{
		std::cout << v << std::endl;
	}
	return 0;
}