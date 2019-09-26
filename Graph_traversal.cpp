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
	size_t vertex_count_,
		   edge_count_;



};

class GraphAdjMatrix : public Graph
{
public:
	GraphAdjMatrix(const size_t vertex_count, bool is_directed) :
		Graph(vertex_count, is_directed)
	{
		adj_matrix_= std::vector<std::vector<int>>(vertex_count, std::vector<int>(vertex_count, 0));

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

	void printGraph() const
	{
		std::cout << "Print\n";
		std::cout << "Vertex_count = " << vertex_count_ << "\n";

		for (auto i : adj_matrix_)
		{
			for (auto j : i)
				std::cout << j << " ";
			std::cout << std::endl;
		}
		for (size_t i = 0; i < vertex_count_; i++)
		{
			std::cout << "Neighbors for " << i << " : ";
			for (auto j : this->getNeighbors(i))
				std::cout << j << " ";
			std::cout << "\n";
		}
	}




private:
	std::vector<std::vector<int>> adj_matrix_;

};

namespace GraphProcessing 
{
	enum VERTEXMARK
	{
		WHITE, GREY, BLACK
	};

	
	void dfs_visit(const Graph& g, const size_t& v, size_t& counter, std::vector<VERTEXMARK>& vertex_marks)
	{
		counter++;
		vertex_marks[v] = GREY;
		for (size_t i : g.getNeighbors(v))
		{
			if (vertex_marks[i] == WHITE)
			{
				dfs_visit(g, i, counter, vertex_marks);
				vertex_marks[i] = BLACK;
			}
		}

	}
	
	
	void getConnectedCount(const Graph& g, const size_t& v, size_t& counter)
	{
		std::vector<VERTEXMARK> vertex_marks(g.getVertexCount(), WHITE);
		dfs_visit(g, v, counter, vertex_marks);
	}

}

int main()
{
	size_t n, v;
	std::cin >> n >> v;
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
	size_t answer = 0;
	GraphProcessing::getConnectedCount(g, v - 1, answer);
	std::cout << answer;
	

}