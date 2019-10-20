#include<iostream>
#include<vector>
#include<exception>
// Task TopSort "https://informatics.mccme.ru/mod/statements/view3.php?id=256&chapterid=166#1"

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

	size_t geEdgeCount() const
	{
		return edge_count_;
	}

	virtual std::vector<Vertex> getNeighbors(const Vertex& v) const = 0;

	virtual void addEdge(const Vertex& start, const Vertex& finish)
	{
		edge_count_++;
	}

	bool is_directed() const
	{
		return is_directed_;
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
	namespace
	{
		enum VertexMark
		{
			WHITE, BLACK, GREY
		};
		typedef size_t Vertex;
		void dfs_visit(const Graph& g, const Vertex vertex, std::vector<VertexMark>& vertex_marks, std::vector<Vertex>& answer, bool& is_cycle)
		{
			vertex_marks[vertex] = GREY;
			for (Vertex i : g.getNeighbors(vertex))
			{
				if (vertex_marks[i] == GREY)
				{
					is_cycle = true;
					return;
				}
				if (vertex_marks[i] == WHITE)
				{
					dfs_visit(g, i, vertex_marks, answer, is_cycle);
				}
			}
			vertex_marks[vertex] = BLACK;
			answer.push_back(vertex);

		}
	}

	std::vector<Vertex> getTopsortedList(const Graph& g)
	{
		std::vector<Vertex> answer;
		bool is_cycle = false;
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), WHITE);
		for (Vertex vertex = 0; vertex < g.getVertexCount(); vertex++)
		{
			if (vertex_marks[vertex] == WHITE)
			{
				dfs_visit(g, vertex, vertex_marks, answer, is_cycle);
			}
			if (is_cycle)
			{
				return std::vector<Vertex>(0);
			}
		}
		return answer;
	}
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, true);
	for (size_t i = 0; i < m; i++)
	{
		Graph::Vertex from, to;
		std::cin >> from >> to;
		g.addEdge(to - 1, from - 1);
	}

	std::vector<Graph::Vertex> answer = GraphProcessing::getTopsortedList(g);
	if (answer.size() == 0)
	{
		std::cout << -1;
	}
	else
	{
		for (auto i : answer)
		{
			std::cout << i + 1 << " ";
		}
	}
	return 0;
}