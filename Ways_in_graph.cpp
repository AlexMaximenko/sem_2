#include<iostream>
#include<vector>
#include<queue>
#define NO_EDGE 0
#define IS_EDGE 1
// Ways in graph "https://informatics.mccme.ru/mod/statements/view.php?id=255#1"


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




class GraphAdjMatrix : public Graph
{
public:
	GraphAdjMatrix(const size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
		adj_matrix_ = std::vector<std::vector<int>>(vertex_count, std::vector<int>(vertex_count, NO_EDGE));

	}

	void addEdge(const Vertex& start, const Vertex& finish) override
	{
		Graph::addEdge(start, finish);
		adj_matrix_[start][finish] = IS_EDGE;
	}

	std::vector<Vertex> getNeighbors(const Vertex& v) const override
	{
		std::vector<Vertex> neighbors;
		for (Vertex neighbor = 0; neighbor < vertex_count_; neighbor++)
		{
			if (adj_matrix_[v][neighbor] == IS_EDGE)
			{
				neighbors.push_back(neighbor);
			}
		}
		return neighbors;
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		size_t count = 0;
		for (Vertex u = 0; u < vertex_count_; u++)
		{
			if (adj_matrix_[v][u] == IS_EDGE)
			{
				count++;
			}
		}
		return count;
	}

private:
	std::vector<std::vector<int>> adj_matrix_;
};

namespace GraphProcessing
{
	enum VertexMark
	{
		WHITE, GREY, BLACK
	};

	typedef size_t Vertex;
	const int WASNT_VISIT = -1;

	namespace
	{
		std::vector<Vertex> getWay(const std::vector<Vertex>& prev, const Vertex& finish)
		{
			std::vector<Vertex> reverse_way;
			std::vector<Vertex> way;
			reverse_way.push_back(finish);
			Vertex cur = finish;
			while (prev[cur] != WASNT_VISIT)
			{
				cur = prev[cur];
				reverse_way.push_back(cur);
			}
			for (size_t i = 0; i < reverse_way.size(); ++i)
			{
				way.push_back(reverse_way[reverse_way.size() - i - 1]);
			}
			return way;

		}
	}
	std::vector<Vertex> getShortestWay(const Graph& g, const Vertex& start, const Vertex& finish)
	{
		std::vector<int> distance(g.getVertexCount(), WASNT_VISIT);
		std::vector<Vertex> prev(g.getVertexCount(), WASNT_VISIT);
		distance[start] = 0;
		prev[start] = WASNT_VISIT;

		std::queue<Vertex> qu;
		qu.push(start);
		while (!qu.empty())
		{
			Vertex cur = qu.front();
			qu.pop();
			for (Vertex i : g.getNeighbors(cur))
			{
				if (distance[i] == WASNT_VISIT)
				{
					distance[i] = distance[cur] + 1;
					prev[i] = cur;
					qu.push(i);
				}
			}
		}
		if (distance[finish] == WASNT_VISIT)
		{
			return  std::vector<Vertex>(0);
		}

		return getWay(prev, finish);
	}

}


int main()
{
	size_t n;
	std::cin >> n;
	GraphAdjMatrix g(n, false);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			size_t is_edge;
			std::cin >> is_edge;
			if (is_edge)
			{
				g.addEdge(i, j);
			}
		}
	}
	size_t start, finish;
	std::cin >> start >> finish;
	std::vector<Graph::Vertex> shortest_way = GraphProcessing::getShortestWay(g, start - 1, finish - 1);
	if (shortest_way.size() == 0)
	{
		std::cout << -1;
	}
	else
	{
		std::cout << shortest_way.size() - 1 << std::endl;
		if (shortest_way.size() != 1)
		{
			for (size_t i : shortest_way)
				std::cout << i + 1 << " ";
		}
	}

	return 0;
}
