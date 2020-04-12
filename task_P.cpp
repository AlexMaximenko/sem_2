#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
template<class T>
class DSU
{
public:
	DSU(const std::vector<T>& elements)
	{
		for (size_t i = 0; i < elements.size(); i++)
		{
			prev_[elements[i]] = elements[i];
			size_[elements[i]] = 1;
		}
		sets_count_ = elements.size();
	}
	DSU(size_t n)
	{
		for (size_t i = 0; i < n; i++)
		{
			prev_[i] = i;
			size_[i] = 0;
		}
		sets_count_ = n;
	}
	T findSet(const T& x)
	{
		return x == prev_[x] ? x : prev_[x] = findSet(prev_[x]);
	}
	void Union(const T& x, const T& y)
	{
		T prev_x = findSet(x);
		T prev_y = findSet(y);
		if (prev_x != prev_y)
		{
			if (size_[prev_x] == size_[prev_y])
			{
				std::swap(prev_x, prev_y);
			}
			prev_[prev_y] = prev_x;
			size_[prev_x] += size_[prev_y];
			sets_count_--;
		}
	}
	size_t getSetsCount() const
	{
		return sets_count_;
	}
private:
	std::map<T, size_t> size_;
	std::map<T, T> prev_;
	size_t sets_count_;
};

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
	std::vector<Edge> getMST(const Graph& g)
	{
		std::vector<Edge> spanning_tree;
		std::vector<Edge> edges = g.getEdges();
		std::sort(edges.begin(), edges.end());
		DSU<Graph::Vertex> dsu(g.getVertexCount());
		for (Edge edge : edges)
		{
			if (dsu.findSet(edge.getFrom()) != dsu.findSet(edge.getTo()))
			{
				spanning_tree.push_back(edge);
				dsu.Union(edge.getFrom(), edge.getTo());
			}
			if (dsu.getSetsCount() == 1)
			{
				break;
			}
		}
		return spanning_tree;
	}
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, false);
	std::vector<Edge> edges;
	for (size_t i = 0; i < m; i++)
	{
		size_t to, from;
		int weight;
		std::cin >> from >> to >> weight;
		g.addEdge(from - 1, to - 1, weight);
	}
	int weight = 0;
	for (Edge edge : GraphProcessing::getMST(g))
	{
		weight += edge.getWeight();
	}
	std::cout << weight << "\n";
	return 0;
}