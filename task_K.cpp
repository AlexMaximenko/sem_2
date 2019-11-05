#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<queue>
#include<math.h>
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
	size_t x;
	size_t y;
};
class Graph
{
public:
	const size_t NO_EDGE = 0;
	const size_t IS_EDGE = 1;
	typedef field Vertex;
	Graph(size_t size_x, size_t size_y, bool is_directed)
	{
		vertex_count_ = size_x * size_y;
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
	GraphAdjList(size_t size_x, size_t size_y, bool is_directed) :
		Graph(size_x, size_y, is_directed)
	{
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				adj_list_[{i, j}] = {};
			}
		}

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
	namespace
	{
		enum VertexMark
		{
			WHITE, GREY, BLACK
		};
		const field WASNT_VISIT(-1, -1);
		const int INF = -1;
		void addCorrectEdges(Graph& g, const std::vector<std::vector<size_t>>& game_map, Graph::Vertex vertex)
		{
			const size_t x = vertex.x;
			const size_t y = vertex.y;
			const size_t size_x = game_map.size();
			const size_t size_y = game_map[0].size();
			size_t i = x;
			if (game_map[x][y] == 1)
			{
				return;
			}
			// moving down
			while (i < size_x - 1 && game_map[i + 1][y] != 1)
			{
				i++;
			}
			i -= (i - x) / 2;
			if (i != x)
			{
				g.addEdge(vertex, { i, y });
			}
			// moving up
			i = x;
			while (i > 0 && game_map[i - 1][y] != 1)
			{
				i--;
			}
			i += (x - i) / 2;
			if (i != x)
			{
				g.addEdge(vertex, { i, y });
			}
			// moving left
			i = y;
			while (i > 0 && game_map[x][i - 1] != 1)
			{
				i--;
			}
			i += (y - i) / 2;
			if (i != y)
			{
				g.addEdge(vertex, { x, i });
			}
			// moving right	
			i = y;
			while (i < size_y - 1 && game_map[x][i + 1] != 1)
			{
				i++;
			}
			i -= (i - y) / 2;
			if (i != y)
			{
				g.addEdge(vertex, { x, i });
			}
		}

	}
	int getMinPresses(Graph& g, const std::vector<std::vector<size_t>>& game_map, const Graph::Vertex& start, const Graph::Vertex& finish)
	{
		std::queue<Graph::Vertex> qu;
		std::map<Graph::Vertex, int> distance;
		for (size_t i = 0; i < game_map.size(); i++)
		{
			for (size_t j = 0; j < game_map[0].size(); j++)
			{
				distance[{i, j}] = INF;
			}
		}
		distance[start] = 0;
		qu.push(start);
		while (!qu.empty() && distance[finish] == INF)
		{
			Graph::Vertex curr = qu.front();
			qu.pop();
			addCorrectEdges(g, game_map, curr);
			for (Graph::Vertex neighbor : g.getNeighbors(curr))
			{
				if (distance[neighbor] == INF)
				{
					distance[neighbor] = distance[curr] + 1;
					qu.push(neighbor);
				}
			}
		}
		return distance[finish];
	}
}


int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, m, true);
	Graph::Vertex start;
	Graph::Vertex finish;
	std::vector<std::vector<size_t>> game_map(n, std::vector<size_t>(m));
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			char c;
			std::cin >> c;
			if (c == 'S')
			{
				start = { i, j };
			}
			if (c == 'T')
			{
				finish = {i, j};
			}
			if (c == '#')
			{
				game_map[i][j] = 1;
			}
			else
			{
				game_map[i][j] = 0;
			}
		}
	}
	std::cout << GraphProcessing::getMinPresses(g, game_map, start, finish);
	return 0;
}