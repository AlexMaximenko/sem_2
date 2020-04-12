#include<iostream>
#include<vector>
#include<map>
#include<set>
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

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int64_t weight = 1)
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

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int64_t weight = 1)
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
		typedef std::vector<Graph::Vertex> PATH;
		const size_t INF = 100000;
		const size_t WASNT_VISITED = -1;
		bool Relax(const Edge& edge, std::vector<int64_t>& distance, std::vector<Graph::Vertex>& prev)
		{
			if (distance[edge.getTo()] > distance[edge.getFrom()] + edge.getWeight())
			{
				distance[edge.getTo()] = distance[edge.getFrom()] + edge.getWeight();
				prev[edge.getTo()] = edge.getFrom();
				return true;
			}
			return false;
		}
		PATH getCycle(const std::vector<Graph::Vertex>& prev, Graph::Vertex vertex)
		{
			PATH way;
			Graph::Vertex curr = vertex;
			for (size_t i = 0; i < prev.size(); i++)
			{
				curr = prev[curr];
			}
			Graph::Vertex cycle_finish = curr;
			way.push_back(curr);
			curr = prev[curr];
			while (prev[curr] != cycle_finish)
			{
				way.push_back(curr);
				curr = prev[curr];
			}
			way.push_back(curr);
			way.push_back(prev[curr]);
			std::reverse(way.begin(), way.end());
			return way;
		}
	}
	PATH findNegativeCycle(const Graph& g, Graph::Vertex source)
	{
		std::vector<int64_t> distance(g.getVertexCount(), 0);
		std::vector<Graph::Vertex> prev(g.getVertexCount(), WASNT_VISITED);
		distance[source] = 0;
		for (size_t i = 0; i < g.getVertexCount() - 1; i++)
		{
			for (Edge edge : g.getEdges())
			{
				if (distance[edge.getFrom()] != INF)
				{
					Relax(edge, distance, prev);
				}
			}
		}
		bool flag = false;
		Graph::Vertex vertex;
		for (Edge edge : g.getEdges())
		{	
			if (distance[edge.getFrom()] != INF)
			{
				flag = Relax(edge, distance, prev);
				if (flag)
				{
					vertex = edge.getTo();
					break;
				}
			}
		}
		if (!flag)
		{
			return PATH(0);
		}
		else
		{
			return getCycle(prev, vertex);
		}
	}
}

int main()
{
	size_t n;
	std::cin >> n;
	GraphAdjList g(n, true);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			int64_t weight;
			std::cin >> weight;
			if (weight != 100000)
			{
				g.addEdge(i, j, weight);
			}
		}
	}
	std::vector<Graph::Vertex> negative_cycle = GraphProcessing::findNegativeCycle(g, 0);
	if (negative_cycle.size() != 0)
	{
		std::cout << "YES\n";
		std::cout << negative_cycle.size() << std::endl;
		for (int i : negative_cycle)
		{
			std::cout << i + 1 << " ";
		}
	}
	else std::cout << "NO";
	return 0;
}