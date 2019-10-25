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
	std::vector<field> getCorrectNeighbors(const size_t& size_x, const size_t& size_y) const
	{
		std::vector<field> correct_neighbors;
		if (x >= 1)
		{
			correct_neighbors.push_back({ x - 1, y });
		}
		if (y >= 1)
		{
			correct_neighbors.push_back({ x, y - 1});
		}
		if (x + 1 < size_x)
		{
			correct_neighbors.push_back({ x + 1, y });
		}
		if (y + 1 < size_y)
		{
			correct_neighbors.push_back({ x, y + 1 });
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
	
	std::vector<std::vector<size_t>> getTable(const Graph& g, const std::vector<std::vector<size_t>>& input_table)
	{
		size_t size_x = input_table.size();
		size_t size_y = input_table[0].size();
		std::map<Graph::Vertex, size_t> distance;
		std::vector<std::vector<size_t>> output_table(size_x, std::vector<size_t>(size_y, 0));
		std::queue<Graph::Vertex> qu;
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				distance[{i, j}] = INF;
				if (input_table[i][j] == 1) // putting cells with a value of 1 at the top of the queue
				{
					distance[{i, j}] = 0;
					qu.push({ i,j });
				}
			}
		}
		while (!qu.empty())
		{
			Graph::Vertex curr = qu.front();
			qu.pop();
			for (Graph::Vertex neighbor : g.getNeighbors(curr))
			{
				if (distance[neighbor] == INF)
				{
					distance[neighbor] = distance[curr] + 1;
					qu.push(neighbor);
				}
			}
		}
		for (size_t i = 0; i < size_x; i++)
		{
			for (size_t j = 0; j < size_y; j++)
			{
				output_table[i][j] = distance[{i, j}];
			}
		}
		return output_table;
	}
}


int main()
{
	size_t n, m;
	std::cin >> n >> m;
	std::vector<std::vector<size_t>> input_table(n, std::vector<size_t>(m, 0));
	GraphAdjList g(n*m, false);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			std::cin >> input_table[i][j];
			Graph::Vertex vertex(i, j);
			for (Graph::Vertex neighbor : vertex.getCorrectNeighbors(n, m))
			{
				g.addEdge(vertex, neighbor);
			}
		}
	}
	std::vector<std::vector<size_t>> output_table = GraphProcessing::getTable(g, input_table);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			std::cout << output_table[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}