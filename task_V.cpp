#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<algorithm>

class Edge
{
public:
	Edge(size_t from, int departure_time, size_t to, int arrival_time)
	{
		from_ = from;
		to_ = to;
		departure_time_ = departure_time;
		arrival_time_ = arrival_time;
	}
	size_t from_, to_;
	int arrival_time_, departure_time_;
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

	virtual void addEdge(const Vertex& start, const int departure_time, const Vertex& finish, const int arrival_time)
	{
		Graph::addEdge(start, finish);
		adj_list_[start].push_back(finish);
		edges.push_back(Edge(start, departure_time, finish, arrival_time));
		if (!is_directed_)
		{
			adj_list_[finish].push_back(start);
			edges.push_back(Edge(finish, arrival_time, start, departure_time));
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
		const int INF = 2e9;
		void Relax(const Edge& edge, std::vector<int>& min_time)
		{
			if (min_time[edge.from_] > edge.departure_time_)
			{
				return;
			}
			if (min_time[edge.to_] > edge.arrival_time_ && min_time[edge.from_] != INF)
			{
				min_time[edge.to_] = edge.arrival_time_;
			}
		}
	}
	std::vector<int> findmin_times(const Graph& g, Graph::Vertex source)
	{
		std::vector<int> min_time(g.getVertexCount(), INF);
		min_time[source] = 0;
		for (size_t i = 0; i < g.getEdges().size(); i++)
		{
			for (Edge edge : g.getEdges())
			{
					Relax(edge, min_time);
			}
		}
		return min_time;
	}

}

int main()
{
	size_t n;
	std::cin >> n;
	GraphAdjList g(n, true);
	size_t source, destination;
	std::cin >> source >> destination;
	size_t m;
	std::cin >> m;
	for (size_t i = 0; i < m; i++)
	{
		size_t from, departure_time, to, arrival_time;
		std::cin >> from >> departure_time >> to >> arrival_time;
		g.addEdge(from - 1, departure_time, to - 1, arrival_time);
	}	
	std::cout << GraphProcessing::findmin_times(g, source - 1)[destination - 1];
	return 0;
}