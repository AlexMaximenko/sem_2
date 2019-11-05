#include<iostream>
#include<vector>
#include<algorithm>


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

	virtual void transposeGraph() = 0;

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

	void transposeGraph() override
	{

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

	void dfsForOrder(const Graph& g, const Graph::Vertex& vertex, std::vector<size_t>& finish_time, std::vector<VertexMark>& vertex_marks, size_t& time, std::vector<Graph::Vertex>& order)
	{
		vertex_marks[vertex] = GREY;
		for (Graph::Vertex neighbor : g.getNeighbors(vertex))
		{
			if (vertex_marks[neighbor] == WHITE)
			{
				dfsForOrder(g, neighbor, finish_time, vertex_marks, time, order);
			}
		}
		vertex_marks[vertex] = BLACK;
		finish_time[vertex] = ++time;
		order.push_back(vertex);
	}

	void setComponentNumber(const Graph& transposed_g, const Graph::Vertex& vertex, std::vector<VertexMark>& vertex_marks, const size_t component_count, std::vector<Graph::Vertex>& vertex_component_number)
	{
		vertex_marks[vertex] = GREY;
		vertex_component_number[vertex] = component_count;
		for (Graph::Vertex neighbor : transposed_g.getNeighbors(vertex))
		{
			if (vertex_marks[neighbor] == WHITE)
			{
				setComponentNumber(transposed_g, neighbor, vertex_marks, component_count, vertex_component_number);
			}
		}
		vertex_marks[vertex] = BLACK;
	}

	void setCorrectOrder(const Graph& g, std::vector<size_t>& finish_time, std::vector<VertexMark>& vertex_marks, size_t& time, std::vector<Graph::Vertex>& order)
	{
		for (Graph::Vertex v = 0; v < g.getVertexCount(); ++v)
		{
			if (vertex_marks[v] == WHITE)
			{
				dfsForOrder(g, v, finish_time, vertex_marks, time, order);
			}
		}
	}
	size_t determineComponents(const Graph& g, const Graph& transposed_g, std::vector<Graph::Vertex>& vertex_component_number)
	{
		size_t time = 0;
		std::vector<size_t> finish_time(g.getVertexCount(), WASNT_VISIT);
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), WHITE);
		std::vector<Graph::Vertex> order;
		setCorrectOrder(g, finish_time, vertex_marks, time, order);	//output time ordering
		vertex_marks.clear();
		vertex_marks = std::vector<VertexMark>(g.getVertexCount(), WHITE);
		size_t component_count = 0;
		for (size_t i = 0; i < g.getVertexCount(); i++)			//dfs for transposed graph
		{
			Graph::Vertex current = order[g.getVertexCount() - i - 1];
			if (vertex_marks[current] == WHITE)
			{
				component_count++;
				setComponentNumber(transposed_g, current, vertex_marks, component_count, vertex_component_number);
			}
		}
		return component_count;
	}
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, true);
	GraphAdjList transoped_g(n, true);
	for (size_t i = 0; i < m; i++)
	{
		Graph::Vertex from, to;
		std::cin >> from >> to;
		g.addEdge(from - 1, to - 1);
		transoped_g.addEdge(to - 1, from - 1);
	}
	std::vector<Graph::Vertex> vertex_component_number(g.getVertexCount(), 0);
	std::cout << GraphProcessing::determineComponents(g, transoped_g, vertex_component_number) << std::endl;
	for (Graph::Vertex i : vertex_component_number)
	{
		std::cout << i << " ";
	}
	return 0;
}