#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<queue>
#include<set>
struct Cell
{
	Cell()
	{
		x = 0;
		y = 0;
	}
	Cell(const int& x_, const int& y_)
	{
		x = x_;
		y = y_;
	}
	Cell(const Cell& Cell_)
	{
		x = Cell_.x;
		y = Cell_.y;
	}
	bool operator <(const Cell& second) const
	{
		return std::make_pair(x, y) < std::make_pair(second.x, second.y);
	}
	bool operator ==(const Cell& second) const
	{
		return std::make_pair(x, y) == std::make_pair(second.x, second.y);
	}
	bool operator !=(const Cell& second) const
	{
		return !(*this == second);
	}
	int x;
	int y;
};

struct KnightMove
{
	KnightMove()
	{
	}
	std::vector<Cell> getMoves() const
	{
		return { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {2, -1}, {2, 1}, {1, -2}, {1, 2} };
	}

};

struct  Desk
{
	Desk(size_t size_x, size_t size_y)
	{
		size_x_ = size_x;
		size_y_ = size_y;
	}
	bool inDesk(int x, int y)
	{
		return !(x < 0 || y < 0 || x >= size_x_ || y >= size_y_);
	}
	size_t size_x_;
	size_t size_y_;
};

class Graph
{
public:
	typedef Cell Vertex;
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

	virtual std::vector<Vertex> getVertices() const = 0;

	virtual size_t getNeighborsCount(const Vertex&  v) const = 0;

protected:
	const size_t NO_EDGE = 0;
	const size_t IS_EDGE = 1;
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
		vertices_.insert(start);
	}

	std::vector<Vertex> getNeighbors(const Vertex& v) const override
	{
		return adj_list_.at(v);
	}

	size_t getNeighborsCount(const Vertex&  v) const override
	{
		return adj_list_.at(v).size();
	}

	std::vector<Vertex> getVertices() const override
	{
		std::vector<Vertex> vertices;
		for (Vertex i : vertices_)
		{
			vertices.push_back(i);
		}
		return vertices;
	}

private:
	std::map<Vertex, std::vector<Vertex>> adj_list_;
	std::set<Vertex> vertices_;
};

std::vector<Cell> correctKnightMoves(Cell cell, Desk desk)
{
	std::vector<Cell> correct_moves;
	std::vector<Cell> moves = KnightMove().getMoves();
	for (Cell i : moves)
	{
		if (desk.inDesk(cell.x + i.x, cell.y + i.y))
		{
			Cell correct_move = { cell.x + i.x, cell.y + i.y };
			correct_moves.push_back(correct_move);
		}
	}
	return correct_moves;
}

namespace GraphProcessing
{
	enum VertexMark
	{
		WHITE, GREY, BLACK
	};
	const Cell WASNT_VISIT(-1, -1);
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
		for (Graph::Vertex vertex : g.getVertices())
		{
			distance[vertex] = INF;
			prev[vertex] = WASNT_VISIT;
		}
		distance[start] = 0;
		qu.push(start);
		while (!qu.empty())
		{
			Graph::Vertex curr = qu.front();
			qu.pop();
			for (Graph::Vertex neighbor : correctKnightMoves(curr, Desk(g.getVertexCount(), g.getVertexCount())))
			{
				if (distance[neighbor] == INF)
				{
					distance[neighbor] = distance[curr] + 1;
					prev[neighbor] = curr;
					qu.push(neighbor);
				}
			}
		}
		return distance[finish] == INF ? std::vector<Graph::Vertex>(0) : getWay(prev, finish);
	}
}

int main()
{
	size_t n;
	std::cin >> n;
	GraphAdjList g(n, false);
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
			for (Graph::Vertex u : correctKnightMoves(vertex, Desk(n, n)))
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

