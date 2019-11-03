#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<map>
const size_t MAX_FLOORS = 1000000;

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
		return weight_ > second.weight_;
	}
	bool operator ==(const Edge& second) const
	{
		return(from_ == second.from_ && to_ == second.to_ && weight_ == second.weight_);
	}
	bool operator !=(const Edge& second) const
	{
		return !(*this == second);
	}
	bool operator >(const Edge& second) const
	{
		return weight_ < second.weight_;;
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

	virtual std::vector<Edge> getNeighbors(const Vertex& v) const = 0;

	virtual size_t getNeighborsCount(const Vertex& v) const = 0;

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int64_t weight = 0)
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
		adj_list_ = std::vector<std::vector<Edge>>(vertex_count);
	}

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int64_t weight = 1)
	{
		Graph::addEdge(start, finish);
		adj_list_[start].push_back({ start, finish, weight });
		edges[{start, finish}] = weight;
		if (!is_directed_)
		{
			adj_list_[finish].push_back({ finish, start, weight });
			edges[{finish, start}] = weight;
		}
	}

	std::vector<Edge> getNeighbors(const Vertex& v) const override
	{
		return adj_list_[v];
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		return adj_list_[v].size();
	}

	std::vector<Edge> getEdges() const override
	{
		std::vector<Edge> edges;
		for (Vertex v = 0; v < getVertexCount(); v++)
		{
			edges.insert(edges.end(), adj_list_[v].begin(), adj_list_[v].end());
		}
		return edges;
	}

	void printGraph()
	{
		for (size_t i = 0; i < vertex_count_; i++)
		{
			for (auto j : getNeighbors(i))
			{
				std::cout << j.getFrom() << " " << j.getTo() << " " << j.getWeight() << std::endl;
			}
		}
	}


private:
	std::vector <std::vector<Edge>> adj_list_;
	std::map<std::pair<Vertex, Vertex>, int64_t> edges;
};

struct distance_estimate
{
	distance_estimate(const Graph::Vertex& vertex, const int64_t& estimate)
	{
		vertex_ = vertex;
		estimate_ = estimate;
	}
	Graph::Vertex vertex_;
	int64_t estimate_;
	bool operator<(const distance_estimate& second) const
	{
		return estimate_ > second.estimate_;
	}
};

namespace GraphProcessing
{
	namespace
	{
		const int64_t INF = 2e12;
		void Relax(const Graph& g, const Graph::Vertex& vertex, std::vector<int64_t>& distance, std::priority_queue<distance_estimate>& current_distances, std::vector<bool>& used)
		{
			for (Edge edge : g.getNeighbors(vertex))
			{
				if (distance[edge.getTo()] > distance[edge.getFrom()] + edge.getWeight())
				{
					distance[edge.getTo()] = distance[edge.getFrom()] + edge.getWeight();
					if (!used[edge.getTo()])
					{
						current_distances.push({ edge.getTo(), distance[edge.getTo()] });
					}
				}
			}
		}
	}
	std::vector<int64_t> findDistances(const Graph& g, const Graph::Vertex& source)
	{
		std::vector<bool> used(g.getVertexCount(), false);
		std::vector<int64_t> distance(g.getVertexCount(), INF);
		std::priority_queue<distance_estimate> current_distances;
		distance[source] = 0;
		current_distances.push({ source, 0 });
		while (!current_distances.empty())
		{
			Graph::Vertex curr = current_distances.top().vertex_;
			current_distances.pop();
			if (!used[curr])
			{
				used[curr] = true;
				Relax(g, curr, distance, current_distances, used);
			}
		}
		return distance;
	}
}

int main()
{
	size_t N, U, D, I, J, L;
	std::cin >> N >> U >> D >> I >> J >> L;
	GraphAdjList g(MAX_FLOORS + L, true);
	for (size_t i = 1; i < MAX_FLOORS; i++)
	{
		g.addEdge(i - 1, i, U);
		g.addEdge(i, i - 1, D);
	}
	for (size_t i = MAX_FLOORS; i < MAX_FLOORS + L; i++)
	{
		size_t num;
		std::cin >> num;
		std::vector<size_t> floors(num);
		for (size_t j = 0; j < num; j++)
		{
			std::cin >> floors[j];
		}
		for (size_t& floor : floors)
		{
			g.addEdge(floor - 1, i, I);
			g.addEdge(i, floor - 1, J);
		}
	}
	std::cout << GraphProcessing::findDistances(g, 0)[N - 1];
	return 0;
}