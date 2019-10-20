#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
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

	size_t getEdgeCount() const
	{
		return edge_count_;
	}

	virtual std::vector<Vertex> getNeighbors(const Vertex& v) const = 0;

	virtual void addEdge(const Vertex& start, const Vertex& finish)
	{
		edge_count_++;
	}

	virtual size_t getNeighborsCount(const Vertex&  v) const = 0;

protected:
	bool is_directed_;
	size_t vertex_count_,
		edge_count_;
};




class GraphAdjList : public Graph
{
public:
	const size_t NO_EDGE = 0;
	const size_t IS_EDGE = 1;
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
private:
	std::vector<std::vector<Vertex>> adj_list_;
};

namespace GraphProcessing
{
	enum VertexMark
	{
		WHITE, BLACK, GREY
	};

	void dfs_visit(const Graph& g, const Graph::Vertex& vertex, std::vector<VertexMark>& vertex_marks)
	{
		vertex_marks[vertex] = GREY;
		for (Graph::Vertex neighbor : g.getNeighbors(vertex))
		{
			if (vertex_marks[neighbor] == WHITE)
			{
				dfs_visit(g, neighbor, vertex_marks);
			}
		}
		vertex_marks[vertex] = BLACK;
	}

	size_t getComponentsCount(const Graph& g)
	{
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), WHITE);
		size_t counter = 0;
		for (Graph::Vertex v = 0; v < g.getVertexCount(); ++v)
		{
			if (vertex_marks[v] == WHITE)
			{
				counter++;
				dfs_visit(g, v, vertex_marks);
			}
		}
		return counter;
	}
}



int main()
{
	size_t n;
	std::cin >> n;
	GraphAdjList g(n, false);
	for (size_t i = 0; i < n; ++i)
	{
		Graph::Vertex from;
		std::cin >> from;
		g.addEdge(from - 1, i);
	}
	std::cout << GraphProcessing::getComponentsCount(g);
	return 0;
}