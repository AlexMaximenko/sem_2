#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
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

	virtual size_t getNeighborsCount(const Vertex& v) const = 0;

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

		void dfsForArticulaitonPoints(const Graph::Vertex& vertex, const Graph& g, size_t& timer, std::vector<VertexMark>& vertex_marks, std::vector<size_t>& entry_time, std::vector<size_t>& f_up, std::vector<Graph::Vertex>& answer, bool is_root)
		{
			vertex_marks[vertex] = GREY;
			entry_time[vertex] = f_up[vertex] = ++timer;
			size_t children = 0;
			for (Graph::Vertex neighbor : g.getNeighbors(vertex))
			{
				if ((vertex_marks[neighbor] == GREY))
				{
					f_up[vertex] = std::min(f_up[vertex], entry_time[neighbor]);
				}
				else
				{
					if (vertex_marks[neighbor] == WHITE)
					{
						children++;
						dfsForArticulaitonPoints(neighbor, g, timer, vertex_marks, entry_time, f_up, answer, false);
						f_up[vertex] = std::min(f_up[vertex], f_up[neighbor]);
						if ((f_up[neighbor] >= entry_time[vertex]) && (!is_root))
						{
							answer.push_back(vertex);
						}
					}
				}
			}
			if ((is_root) && (children > 1))
			{
				answer.push_back(vertex);
			}
			vertex_marks[vertex] = BLACK;
		}
	}

	std::vector<Graph::Vertex> getArticulationPoints(const Graph& g)
	{
		std::vector<Graph::Vertex> answer;
		const size_t INF = -1;
		std::vector<size_t> f_up(g.getVertexCount(), INF);
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), WHITE);
		std::vector<size_t> entry_time(g.getVertexCount(), INF);
		size_t timer = 0;
		for (Graph::Vertex vertex = 0; vertex < g.getVertexCount(); vertex++)
		{
			if (vertex_marks[vertex] == WHITE)
			{
				dfsForArticulaitonPoints(vertex, g, timer, vertex_marks, entry_time, f_up, answer, true);
			}
		}
		return answer;
	}
}

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	GraphAdjList g(n, false);
	for (size_t i = 0; i < m; i++)
	{
		size_t to, from;
		std::cin >> to >> from;
		g.addEdge(from - 1, to - 1);
	}
	std::vector<Graph::Vertex> articulation_points = GraphProcessing::getArticulationPoints(g);
	std::sort(articulation_points.begin(), articulation_points.end());
	articulation_points.erase(std::unique(articulation_points.begin(), articulation_points.end()), articulation_points.end());
	std::cout << articulation_points.size() << "\n";
	for (Graph::Vertex i : articulation_points)
	{
		std::cout << i + 1 << "\n";

	}
	return 0;
}