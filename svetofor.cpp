#include<iostream>
#include<vector>
#include<queue>

//Вершина - числа, идущие подряд
//Изначально знаем количество вершин

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

	void addEdge(const Vertex& start, const Vertex& finish) override
	{
		
		adj_list_[start].push_back(finish);
		if (!is_directed_)
			adj_list_[finish].push_back(start);
		edge_count_++;
	}

	std::vector<Vertex> getNeighbors(const Vertex& v) const
	{
		return adj_list_[v];
	}

	int getNeighborsCount(const Vertex&  v) const
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
		white, grey, black
	};

	typedef size_t Vertex;
	/*std::vector<std::vector<Vertex>> getConnectedComponents(const Graph& g) // Каждая компонента связности - отдельный вектор
	{
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), white);
		std::vector < std::vector<Vertex>> components;

		for (Vertex v = 0; v < g.getVertexCount(); v++)
			if (vertex_marks[v] == white)
			{
				components.push_back(std::vector<Vertex>(0));
				dfs(g, v, vertex_marks, components[components.size() - 1]);

			}
					
	}

	void dfs(const Graph&, const Vertex& v, std::vector<VertexMark>& vertex_marks, std::vector<Vertex>& component)
	{

	}*/



}

int main()
{
	int n, m;
	std::cin >> n;
	std::cin >> m;
	GraphAdjList g(n, false);
	for (size_t i = 0; i < m; i++)
	{
		
		size_t a, b;
		std::cin >> a;
		std::cin >> b;
		g.addEdge(a-1, b-1);
	}
	for (int i = 0; i < n; i++)
	{
			std::cout << g.getNeighborsCount(i) << " ";
	}
	//g.printGraph();

}