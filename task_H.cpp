#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<queue>

struct field
{
	field()
	{
		x = 0;
		y = 0;
	}
	field(const size_t& x_, const size_t& y_)
	{
		x = x_;
		y = y_;
	}
	field(const field& field_)
	{
		x = field_.x;
		y = field_.y;
	}
	bool operator <(const field& second) const
	{
		if (this->x < second.x)
		{
			return true;
		}
		else
		{
			if (this->x == second.x && this->y < second.y)
			{
				return true;
			}
			return false;
		}
	}
	bool operator ==(const field& second) const
	{
		if (this->x == second.x && this->y == second.y)
		{
			return  true;
		}
		return false;
	}
	bool operator !=(const field& second) const
	{
		if (*this == second)
		{
			return false;
		}
		return true;
	}
	std::vector<field> getCorrectNeighbors(const size_t& size) const
	{
		std::vector<field> correct_neighbors;
		if (x >= 2 && y >= 1)
		{
			correct_neighbors.push_back({ x - 2, y - 1 });
		}
		if (x >= 2 && y + 1 < size)
		{
			correct_neighbors.push_back({ x - 2, y + 1 });
		}
		if (x + 2 < size && y >= 1)
		{
			correct_neighbors.push_back({ x + 2, y - 1 });
		}
		if (x + 2 < size && y + 1 < size)
		{
			correct_neighbors.push_back({ x + 2, y + 1 });
		}
		if (x >= 1 && y >= 2)
		{
			correct_neighbors.push_back({ x - 1, y - 2 });
		}
		if (x >= 1 && y + 2 < size)
		{
			correct_neighbors.push_back({ x - 1, y + 2 });
		}
		if (x + 1 < size && y >= 2)
		{
			correct_neighbors.push_back({ x + 1, y - 2 });
		}
		if (x + 1 < size && y + 2 < size)
		{
			correct_neighbors.push_back({ x + 1, y + 2 });
		}
		return correct_neighbors;
	}
	size_t x;
	size_t y;
};
class Graph
{
public:
	const size_t NO_EDGE = 0;
	const size_t IS_EDGE = 1;
	typedef field Vertex;
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
	GraphAdjList(size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
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
		return adj_list_.at(v);
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		return adj_list_.at(v).size();
	}

private:
	std::map<Vertex, std::vector<Vertex>> adj_list_;
};
namespace GraphProcessing
{
	enum VertexMark
	{
		WHITE, GREY, BLACK
	};
	const field WASNT_VISIT(-1, -1);
	const size_t INF = -1;
	namespace
	{
		std::vector<Graph::Vertex> getWay(const std::map<Graph::Vertex, Graph::Vertex>& prev, const Graph::Vertex& finish)
		{
			std::vector<Graph::Vertex> way;
			Graph::Vertex curr = finish;
			while (prev.at(curr) != WASNT_VISIT)
			{
				way.push_back(curr);
				curr = prev.at(curr);
			}
			way.push_back(curr);
			std::reverse(way.begin(), way.end());
			return way;
		}
	}
	std::vector<Graph::Vertex> getShortestWay(const Graph& g, const Graph::Vertex& start, const Graph::Vertex& finish)
	{
		std::queue<Graph::Vertex> qu;
		std::map<Graph::Vertex, Graph::Vertex> prev;
		std::map<Graph::Vertex, size_t> distance;
		for (size_t i = 0; i < sqrt(g.getVertexCount()); i++)
		{
			for (size_t j = 0; j < sqrt(g.getVertexCount()); j++)
			{
				Graph::Vertex vertex(i, j);
				distance[vertex] = INF;
				prev[vertex] = WASNT_VISIT;
			}
		}
		distance[start] = 0;
		qu.push(start);
		while (!qu.empty())
		{
			Graph::Vertex curr = qu.front();
			qu.pop();
			for (Graph::Vertex neighbor : curr.getCorrectNeighbors(g.getVertexCount()))
			{
				if (distance[neighbor] == INF)
				{
					distance[neighbor] = distance[curr] + 1;
					prev[neighbor] = curr;
					qu.push(neighbor);
				}
			}
		}
		if (distance[finish] == INF)
		{
			return  std::vector<Graph::Vertex>(0);
		}
		else
		{
			return getWay(prev, finish);
		}
	}
}


int main()
{
	size_t n;
	std::cin >> n;
	GraphAdjList g(n * n, false);
	Graph::Vertex start, finish;
	std::cin >> start.x >> start.y;
	start.x -= 1;
	start.y -= 1;
	std::cin >> finish.x >> finish.y;
	finish.x -= 1;
	finish.y -= 1;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			Graph::Vertex vertex(i, j);
			for (Graph::Vertex u : vertex.getCorrectNeighbors(n))
			{
				g.addEdge(vertex, u);
			}
		}
	}
	std::vector<Graph::Vertex> shortest_way = GraphProcessing::getShortestWay(g, start, finish);
	if (shortest_way.size() == 0)
	{
		std::cout << -1;
	}
	else 
	{
		std::cout << shortest_way.size() - 1 << std::endl;
		for (Graph::Vertex vertex : shortest_way)
		{
			std::cout << vertex.x + 1 << " " << vertex.y + 1 << std::endl;
		}
	}
	return 0;
}