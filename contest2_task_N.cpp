#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<map>
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

	void setWeight(const int64_t& new_weight)
	{
		weight_ = new_weight;
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

	virtual void setWeight(const Vertex& from, const Vertex& to, const int64_t& new_weight) = 0;

	size_t geEdgeCount() const
	{
		return is_directed_ ? edge_count_ : edge_count_;
	};

	virtual std::vector<Vertex> getNeighbors(const Vertex& v) const = 0;

	virtual size_t getNeighborsCount(const Vertex& v) const = 0;

	virtual void addEdge(const Vertex& start, const Vertex& finish, const int64_t& weight = 0)
	{
		edge_count_++;
	}

	virtual int64_t getWeight(const Vertex& from, const Vertex& to) = 0;

protected:
	int64_t NO_EDGE = 0;
	int64_t IS_EDGE = 1;
	bool is_directed_;
	size_t vertex_count_,
		edge_count_;
};

class GraphAdjMatrix : public Graph
{
public:
	GraphAdjMatrix(const size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
		adj_matrix_ = std::vector<std::vector<int64_t>>(vertex_count, std::vector<int64_t>(vertex_count, NO_EDGE));

	}

	void addEdge(const Vertex& start, const Vertex& finish, const int64_t& weight = 1) override
	{
		Graph::addEdge(start, finish);
		adj_matrix_[start][finish] = weight;
		if (!is_directed_)
		{
			adj_matrix_[finish][start] = weight;
		}
	}

	std::vector<Vertex> getNeighbors(const Vertex& v) const override
	{
		std::vector<Vertex> neighbors;
		for (Vertex i = 0; i < vertex_count_; i++)
		{
			if (adj_matrix_[v][i])
			{
				neighbors.push_back(i);
			}
		}
		return neighbors;
	}

	int64_t getWeight(const Vertex& from, const Vertex& to) override
	{
		return adj_matrix_[from][to];
	}

	void setWeight(const Vertex& from, const Vertex& to, const int64_t& new_weight) override
	{
		adj_matrix_[from][to] = new_weight;
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		size_t count = 0;
		for (Vertex i = 0; i < adj_matrix_[v].size(); i++)
		{
			if (adj_matrix_[v][i] == IS_EDGE)
			{
				count++;
			}
		}
		return count;
	}

	void printGraph() const
	{
		std::cout << "Print\n";
		std::cout << "Vertex_count = " << vertex_count_ << "\n";

		for (auto i : adj_matrix_)
		{
			for (auto j : i)
				std::cout << j << " ";
			std::cout << std::endl;
		}
		for (size_t i = 0; i < vertex_count_; i++)
		{
			std::cout << "Neighbors for " << i << " : ";
			for (auto j : this->getNeighbors(i))
				std::cout << j << " ";
			std::cout << "\n";
		}
	}

private:
	std::vector<std::vector<int64_t>> adj_matrix_;

};

namespace GraphProcessing
{
	namespace
	{
		enum VertexMark
		{
			WHITE, BLACK, GRAY
		};
		const int INF = 1000000000;
	}
	int64_t dfsVisit(Graph& g, std::vector<VertexMark>& color, const Graph::Vertex& vertex, int64_t min)
	{
		if (vertex == g.getVertexCount() - 1)
		{
			return min;
		}
		color[vertex] = GRAY;
		for (Graph::Vertex v : g.getNeighbors(vertex))
		{
			if (color[v] == WHITE)
			{
				int64_t result = dfsVisit(g, color, v, std::min(min, g.getWeight(vertex, v)));
				if (result)
				{
					g.setWeight(vertex, v, g.getWeight(vertex, v) - result);
					g.setWeight(v, vertex, g.getWeight(v, vertex) + result);
					return result;
				}
			}
		}
		color[vertex] = BLACK;
		return 0;
	}

	int64_t getMaximumFLow(Graph& g)
	{
		int64_t current = INF, max_flow = 0;
		while (current)
		{
			std::vector<VertexMark> color(g.getVertexCount(), WHITE);
			current = dfsVisit(g, color, 0, INF);
			max_flow += current;
		}
		return max_flow;
	}
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjMatrix g(n, true);
	for (size_t i = 0; i < m; i++)
	{
		size_t from, to, capacity;
		std::cin >> from >> to >> capacity;
		g.addEdge(from - 1, to - 1, capacity);
	}
	std::cout << GraphProcessing::getMaximumFLow(g);
}