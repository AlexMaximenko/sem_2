#include<iostream>
#include<vector>
//Graph_traversal "https://informatics.mccme.ru/mod/statements/view3.php?id=256&chapterid=164#1"

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

	virtual size_t getNeighborsCount(const Vertex&  v) const = 0;
protected:
	bool is_directed_;
	size_t vertex_count_ = 0,
		edge_count_ = 0;
};

class GraphAdjList : public Graph
{

public:
	const size_t NO_EDGE = 0;
	const size_t IS_EDGE = 1;

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
			std::cout << "Neighbors for vertex " << i + 1 << std::endl;
			for (auto k : adj_list_[i])
				std::cout << k + 1 << " ";
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

	typedef std::vector<std::vector<Graph::Vertex>> Components;

	void dfs_visit(const Graph& g, const Graph::Vertex& vertex, std::vector<VertexMark>& vertex_marks, std::vector<std::vector<Graph::Vertex>>& components)
	{
		vertex_marks[vertex] = GREY;
		components.back().push_back(vertex);
		for (Graph::Vertex neighbor : g.getNeighbors(vertex))
		{
			if (vertex_marks[neighbor] == WHITE)
			{
				dfs_visit(g, neighbor, vertex_marks, components);
			}
		}
		vertex_marks[vertex] = BLACK;
	}

	Components getComponents(const Graph& g)
	{
		Components connectivity_components;
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), WHITE);
		for (Graph::Vertex i = 0; i < g.getVertexCount(); i++)
		{
			if (vertex_marks[i] == WHITE)
			{
				connectivity_components.push_back(std::vector<Graph::Vertex>(0));
				dfs_visit(g, i, vertex_marks, connectivity_components);
			}
		}
		return connectivity_components;
	}
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, false);
	for (size_t i = 0; i < m; i++)
	{
		Graph::Vertex from, to;
		std::cin >> from >> to;
		g.addEdge(from - 1, to - 1);
	}
	std::vector<std::vector<Graph::Vertex>> components = GraphProcessing::getComponents(g);
	std::cout << components.size() << std::endl;
	for (size_t i = 0; i < components.size(); ++i)
	{
		std::cout << components[i].size() << std::endl;
		for (Graph::Vertex vertex : components[i])
		{
			std::cout << vertex + 1 << " ";
		}
		std::cout << std::endl;
	}
}