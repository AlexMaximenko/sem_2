#include<iostream>
#include<vector>
#include<queue>
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
		adj_matrix_ = std::vector<std::vector<int>>(vertex_count, std::vector<int>(vertex_count, 0));

	}

	void addEdge(const Vertex& start, const Vertex& finish) override
	{
		Graph::addEdge(start, finish);
		adj_matrix_[start][finish] = 1;
	}

	std::vector<Vertex> getNeighbors(const Vertex& v) const override
	{
		std::vector<Vertex> neighbors;
		for (size_t i = 0; i < vertex_count_; i++)
			if (adj_matrix_[v][i])
				neighbors.push_back(i);
		return neighbors;
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		size_t count = 0;
		for (auto i : adj_matrix_[v])
			count += i;
		return count;
	}

private:
	std::vector<std::vector<int>> adj_matrix_;
};

namespace GraphProcessing
{
	enum VertexMark
	{
		white, grey, black
	};

	typedef size_t Vertex;

	std::pair<int, std::vector<Vertex>> getShortestWay(const Graph& g, const Vertex& start, const Vertex& finish)
	{
		std::vector<int> distance(g.getVertexCount(), -1);
		std::vector<Vertex> prev(g.getVertexCount(), -1);
		distance[start] = 0;
		prev[start] = -1;

		std::queue<Vertex> qu;
		qu.push(start);
		while (!qu.empty()) 
		{
			Vertex cur = qu.front();
			qu.pop();
			for (auto i : g.getNeighbors(cur))
			{
				if (distance[i] == -1)
				{
					distance[i] = distance[cur] + 1;
					prev[i] = cur;
					qu.push(i);
				}
			}
		}
		if (distance[finish] == -1)
			return std::make_pair(-1, std::vector<Vertex>(0));
		std::vector<Vertex> way;
		way.push_back(finish);
		Vertex cur = finish;
		while (prev[cur] != -1)
		{
			cur = prev[cur];
			way.push_back(cur);
		}
		return std::make_pair(distance[finish], way);
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
			size_t a;
			std::cin >> a;
			if (a)
				g.addEdge(i, j);
		}
	}
	size_t start, finish;
	std::cin >> start >> finish;
	std::pair<int, std::vector<size_t>> answer = GraphProcessing::getShortestWay(g, start - 1, finish - 1);
	if (answer.first == -1)
		std::cout << -1;
	else
		if (answer.first == 0)
			std::cout << 0;
		else
		{
			std::cout << answer.first << "\n";
			for (int i = answer.second.size() - 1; i >= 0; i--)
				std::cout << answer.second[i] + 1 << " ";
		}

	return 0;
}
