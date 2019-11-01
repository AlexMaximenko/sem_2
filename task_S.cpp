#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<algorithm>

class Edge
{
public:
	Edge(size_t from, size_t to, int weight)
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
	size_t getWeight() const
	{
		return weight_;
	}
private:
	size_t from_, to_;
	int weight_;
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
		const size_t INF = -1;
		void Relax(const Edge& edge, std::vector<int>& distance)
		{
			if (distance[edge.getTo()] > distance[edge.getFrom()] + edge.getWeight())
			{
				distance[edge.getTo()] = distance[edge.getFrom()] + edge.getWeight();
			}
		}
	}
	std::vector<int> findDistances(const Graph& g, Graph::Vertex source)
	{
		std::vector<int> distance(g.getVertexCount(), INF);
		distance[0] = 0;
		for (size_t i = 0; i < g.getVertexCount() - 1; i++)
		{
			for (Edge edge : g.getEdges())
			{
				if (distance[edge.getFrom()] != -1)
				{
					Relax(edge, distance);
				}
			}
		}
		return distance;
	}

}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, true);
	for (size_t i = 0; i < m; i++)
	{
		size_t from, to;
		int weight;
		std::cin >> from >> to >> weight;
		g.addEdge(from - 1, to - 1, weight);
	}
	for (int i : GraphProcessing::findDistances(g, 0))
	{
		std::cout << ((i != -1) ? i : 30000) << " ";
	}
	return 0;
}