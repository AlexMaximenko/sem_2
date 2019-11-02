#include<iostream>
#include<vector>
#include<algorithm>

class Edge
{
public:
	Edge(size_t from, size_t to, int64_t weight)
	{
		from_ = from;
		to_ = to;
		weight_ = weight;
	}
	bool operator <(const Edge& second) const
	{
		return ((weight_ < second.weight_));
	}

	size_t getTo() const
	{
		return to_;
	}
	size_t getFrom() const
	{
		return from_;
	}
	int64_t getWeight() const
	{
		return weight_;
	}
private:
	size_t from_, to_;
	int64_t weight_;
};

class Graph
{
public:
	typedef size_t Vertex;
	Graph(size_t vertex_count, bool is_directed = false)
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
		return is_directed_ ? edge_count_ : edge_count_;
	};

	virtual std::vector<Vertex> getNeighbors(const Vertex& v) const = 0;

	virtual size_t getNeighborsCount(const Vertex& v) const = 0;

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int weight = 0)
	{
		edge_count_++;
	}
	virtual std::vector<Edge> getEdges() const = 0;

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

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int weight = 1)
	{
		Graph::addEdge(start, finish);
		adj_list_[start].push_back(finish);
		edges.push_back(Edge(start, finish, weight));
		if (!is_directed_)
		{
			adj_list_[finish].push_back(start);
			edges.push_back(Edge(finish, start, weight));
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

	std::vector<Edge> getEdges() const override
	{
		return edges;
	}

private:
	std::vector <std::vector<Vertex>> adj_list_;
	std::vector<Edge> edges;
};
namespace GraphProcessing
{
	namespace
	{
		const int64_t INF = 2e9;
	}
	std::vector<std::vector<int64_t>> findAllDistances(const Graph& g)
	{
		std::vector<std::vector<int64_t>> distances(g.getVertexCount(), std::vector<int64_t>(g.getVertexCount(), INF));
		for (Edge edge : g.getEdges())
		{
			distances[edge.getFrom()][edge.getTo()] = edge.getWeight();
		}
		for (size_t k = 0; k < g.getVertexCount(); k++)
		{
			for (size_t i = 0; i < g.getVertexCount(); i++)
			{
				for (size_t j = 0; j < g.getVertexCount(); j++)
				{
					distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
				}
			}
		}
		return distances;
	}

}

int main()
{
	size_t n, s, t;
	std::cin >> n >> s >> t;
	GraphAdjList g(n, true);
	for (size_t i = 0; i < n; i++)
	{
		for(size_t j = 0; j < n; j++)
		{
			int64_t weight;
			std::cin >> weight;
			if (weight != -1)
			{
				g.addEdge(i, j, weight);
			}
		}
	}
	int64_t distance = GraphProcessing::findAllDistances(g)[s - 1][t - 1];
	std::cout << (distance == GraphProcessing::INF? -1: distance);
	return 0;
}