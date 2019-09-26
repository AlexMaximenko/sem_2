#include<iostream>
#include<vector>
#include<queue>
#include<deque>
// Ways in graph "https://informatics.mccme.ru/mod/statements/view.php?id=255#1"

#define WASNT_VISIT -1
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

	std::deque<Vertex> getWay(const std::vector<Vertex>& prev, const Vertex& finish)
	{
		std::deque<Vertex> way;
		way.push_front(finish);
		Vertex cur = finish;
		while (prev[cur] != WASNT_VISIT)
		{
			cur = prev[cur];
			way.push_front(cur);
		}
		return way;

	}

	std::pair<int, std::deque<Vertex>> getShortestWay(const Graph& g, const Vertex& start, const Vertex& finish)
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
			return std::make_pair(WASNT_VISIT, std::deque<Vertex>(0));
		}
		
		return std::make_pair(distance[finish], getWay(prev, finish));
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
	std::pair<int, std::deque<size_t>> answer = GraphProcessing::getShortestWay(g, start - 1, finish - 1);
	if (answer.first == WASNT_VISIT)
		std::cout << WASNT_VISIT;
	else
		{
			std::cout << answer.first << "\n";
			if (answer.first > 0)
			{
				for (size_t i : answer.second)
					std::cout << i + 1 << " ";
			}
		}
	return 0;
}
