#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#define NO_EDGE 0
#define IS_EDGE 1
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
	typedef size_t Vertex;
	const int WASNT_VISIT = 2000000;
	
	void dfs_visit(const Graph& g, const size_t& vertex, std::vector<VertexMark>& vertex_marks, std::vector<Vertex>& previous, Vertex& cycle_start)
	{
		vertex_marks[vertex] = GREY;
		for (Vertex neighbor : g.getNeighbors(vertex))
		{
			if (vertex_marks[neighbor] == WHITE)
			{
				previous[neighbor] = vertex;
				dfs_visit(g, neighbor, vertex_marks, previous, cycle_start);
				if (cycle_start != WASNT_VISIT)
				{
					return;
				}
			}
			else
			{
				if (vertex_marks[neighbor] == GREY)
				{
					cycle_start = neighbor;
					previous[neighbor] = vertex;
					return;
				}
			}
		}
		vertex_marks[vertex] = BLACK;
	}
	
	std::vector<Vertex> getCycle(const Graph& g)
	{
		std::vector<Vertex> previous(g.getVertexCount(), WASNT_VISIT); 
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), WHITE);
		Vertex cycle_start = WASNT_VISIT;
		for (Vertex v = 0; v < g.getVertexCount(); ++v)
		{
			if (vertex_marks[v] == WHITE)
			{
				dfs_visit(g, v, vertex_marks, previous, cycle_start);
			}
			if (cycle_start != WASNT_VISIT)
			{
				break;
			}
		}
		if (cycle_start == WASNT_VISIT)
		{
			return std::vector<Vertex>(0);
		}
		std::vector<Vertex> cycle;
		cycle.push_back(previous[cycle_start]);
		Vertex current = previous[cycle_start];
		while (current != cycle_start)
		{
			cycle.push_back(previous[current]);
			current = previous[current];
		}
		std::reverse(cycle.begin(), cycle.end());
		return cycle;
	}
}

	

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, true);
	for (size_t i = 0; i < m; ++i)
	{
		Graph::Vertex from, to;
		std::cin >> from >> to;
		g.addEdge(from - 1, to - 1);
	}
	std::vector<Graph::Vertex> cycle = GraphProcessing::getCycle(g);
	if (cycle.size() == 0)
	{
		std::cout << "NO";
	}
	else
	{
		std::cout << "YES\n";
		for (Graph::Vertex i : cycle)
		{
			std::cout << i + 1 << " ";
		}
	}
	return 0;
}