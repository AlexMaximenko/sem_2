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
		void Relax(const Edge& edge, std::vector<int>& distance_curr, std::vector<int>& distance_prev, std::vector<size_t>& way_length)
		{
			if (distance_curr[edge.getTo()] > distance_prev[edge.getFrom()] + edge.getWeight())
			{
				way_length[edge.getTo()] = way_length[edge.getFrom()] + 1;
				distance_curr[edge.getTo()] = distance_prev[edge.getFrom()] + edge.getWeight();
			}
		}
	}
	std::vector<int> findDistances(const Graph& g, Graph::Vertex source, size_t max_way_length)
	{
		std::vector<int> distance_curr(g.getVertexCount(), INF);
		std::vector<int> distance_prev(g.getVertexCount(), INF);
		std::vector<size_t> way_length_curr(g.getVertexCount(), 0);
		std::vector<size_t> way_length_prev(g.getVertexCount(), 0);
		distance_curr[source] = 0;
		distance_prev[source] = 0;
		for (size_t current_length = 0; current_length < max_way_length; current_length++)
		{
			for (Edge edge : g.getEdges())
			{
				if (distance_prev[edge.getFrom()] != INF && way_length_prev[edge.getFrom()] <= current_length)
				{
					Relax(edge, distance_curr, distance_prev, way_length_curr);
				}
			}
			distance_prev = distance_curr;
			way_length_prev = way_length_curr;
		}
		return distance_curr;
	}

}

int main()
{
	size_t n, m, k, s, f;
	std::cin >> n >> m >> k >> s >> f;
	GraphAdjList g(n, true);
	for (size_t i = 0; i < m; i++)
	{
		size_t from, to;
		int weight;
		std::cin >> from >> to >> weight;
		g.addEdge(from - 1, to - 1, weight);
	}
	std::cout << GraphProcessing::findDistances(g, s - 1, k)[f - 1];
	return 0;
}