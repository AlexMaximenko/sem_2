#include<iostream>
#include<vector>
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
		FIRST_PART, SECOND_PART, NO_PART
	};

	bool checkForBipartite(const Graph& g)
	{
		bool is_bipartite = true;
		std::vector<Graph::Vertex> color(g.getVertexCount(), NO_PART); // NO_PART - wasn't visited
		std::queue<Graph::Vertex> qu;
		for (Graph::Vertex v = 0; v < g.getVertexCount(); ++v) // In case there are several connected component
		{
			if (color[v] == NO_PART)
			{
				color[v] = FIRST_PART;
				qu.push(v);
				while (!qu.empty())
				{
					Graph::Vertex cur = qu.front();
					qu.pop();
					VertexMark current_color = color[cur] == FIRST_PART ? SECOND_PART : FIRST_PART;
					for (Graph::Vertex i : g.getNeighbors(cur))
					{
						if (color[i] == NO_PART)
						{
							color[i] = current_color;
							qu.push(i);
						}
						if (color[i] == color[cur])
						{
							is_bipartite = false;
							break;
						}
					}
				}
			}
		}
		return is_bipartite;
	}
}


int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, false);
	for (size_t i = 0; i < m; ++i)
	{
		Graph::Vertex from, to;
		std::cin >> from >> to;
		g.addEdge(from - 1, to - 1);
	}

	std::cout << (GraphProcessing::checkForBipartite(g) ? "YES" : "NO");

	return 0;
}