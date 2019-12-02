#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
// Ways in graph "https://informatics.mccme.ru/mod/statements/view.php?id=255#1"


class Graph
{
public:
	const size_t NO_EDGE = 0;
	const size_t IS_EDGE = 1;
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

class GraphAdjList : public Graph
{

public:
	GraphAdjList(const size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
		adj_list_ = std::vector<std::vector<Vertex>>(vertex_count);
	}

	void addEdge(const Vertex& start, const Vertex& finish)
	{
		Graph::addEdge(start, finish);
		adj_list_[start].push_back(finish);
		if (!is_directed_)
		{
			adj_list_[finish].push_back(start);
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

	void printGraph() const
	{
		std::cout << "Print\n";
		std::cout << vertex_count_;
		for (size_t i = 0; i < vertex_count_; i++)
		{
			for (auto k : adj_list_[i])
				std::cout << k << " ";
			std::cout << std::endl;
		}
	}

private:
	std::vector<std::vector<Vertex>> adj_list_;
};




namespace GraphProcessing
{
	enum VertexMark
	{
		WHITE, GREY, BLACK
	};
	const int WASNT_VISIT = -1;
	const int INF = -1;
	typedef std::vector<Graph::Vertex> PATH;
	namespace
	{
		std::vector<Graph::Vertex> getWay(const std::vector<Graph::Vertex>& prev, const Graph::Vertex & finish)
		{
			PATH way;
			way.push_back(finish);
			Graph::Vertex cur = finish;
			while (prev[cur] != WASNT_VISIT)
			{
				cur = prev[cur];
				way.push_back(cur);
			}
			std::reverse(way.begin(), way.end());
			return way;
		}
	}
	PATH getShortestWay(const Graph& g, const Graph::Vertex& start, const Graph::Vertex& finish)
	{
		std::vector<int> distance(g.getVertexCount(), INF);
		std::vector<Graph::Vertex> prev(g.getVertexCount(), WASNT_VISIT);
		distance[start] = 0;
		prev[start] = WASNT_VISIT;
		std::queue<Graph::Vertex> qu;
		qu.push(start);
		while (!qu.empty())
		{
			Graph::Vertex cur = qu.front();
			qu.pop();
			for (Graph::Vertex i : g.getNeighbors(cur))
			{
				if (distance[i] == INF)
				{
					distance[i] = distance[cur] + 1;
					prev[i] = cur;
					qu.push(i);
				}
			}
		}
		if (distance[finish] == INF)
		{
			return  PATH(0);
		}
		return getWay(prev, finish);
	}
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	Graph::Vertex start, finish;
	std::cin >> start >> finish;
	GraphAdjList g(n, false);
	for (size_t i = 0; i < m; i++)
	{
		Graph::Vertex from, to;
		std::cin >> from >> to;
		g.addEdge(from - 1, to - 1);
	}
	GraphProcessing::PATH shortest_way = GraphProcessing::getShortestWay(g, start - 1, finish - 1);
	if (shortest_way.size() == 0)
	{
		std::cout << -1;
	}
	else
	{
		std::cout << shortest_way.size() - 1 << std::endl;
		{
			for (Graph::Vertex i : shortest_way)
			{
				std::cout << i + 1 << " ";
			}
		}
	}

	return 0;
}