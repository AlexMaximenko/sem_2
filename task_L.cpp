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
		typedef size_t Vertex;

		enum VertexMark
		{
			WHITE, BLACK, GREY
		};

		void dfsForBridges(const Vertex& vertex, const Graph& g, size_t& timer, std::vector<VertexMark>& vertex_marks, std::vector<size_t>& discovery_time, std::vector<size_t>& f_up, std::vector<std::pair<Vertex, Vertex>>& answer, const Vertex& parent = 0)
		{
			vertex_marks[vertex] = GREY;
			discovery_time[vertex] = f_up[vertex] = ++timer;
			for (Vertex neighbor : g.getNeighbors(vertex))
			{
				if ((vertex_marks[neighbor] == GREY) && (neighbor != parent))
				{
					f_up[vertex] = std::min(f_up[vertex], discovery_time[neighbor]);
				}
				else
				{
					if (vertex_marks[neighbor] == WHITE)
					{
						dfsForBridges(neighbor, g, timer, vertex_marks, discovery_time, f_up, answer, vertex);
						f_up[vertex] = std::min(f_up[vertex], f_up[neighbor]);
					}
				}
				if (f_up[neighbor] > discovery_time[vertex])
				{
					answer.push_back(std::minmax(neighbor + 1, vertex + 1));
				}

			}
			vertex_marks[vertex] = BLACK;

		}
	}

	std::vector<std::pair<Vertex, Vertex>> getBridges(const Graph& g)
	{
		std::vector<std::pair<Vertex, Vertex>> answer;
		const size_t INF = (size_t)(~0);
		std::vector<size_t> f_up(g.getVertexCount(), INF);
		std::vector<VertexMark> vertex_marks(g.getVertexCount(), WHITE);
		std::vector<size_t> discovery_time(g.getVertexCount(), INF);
		size_t timer = 0;
		for (Vertex vertex = 0; vertex < g.getVertexCount(); vertex++)
		{
			if (vertex_marks[vertex] == WHITE)
			{
				dfsForBridges(vertex, g, timer, vertex_marks, discovery_time, f_up, answer);
			}
		}
		return answer;
	}
}

int main()
{
	size_t n, m;
	std::map<std::pair<size_t, size_t>, int> edges;
	std::cin >> n >> m;
	GraphAdjList g(n, false);
	for (size_t i = 0; i < m; i++)
	{
		size_t to, from;
		std::cin >> to >> from;
		if (edges.find(std::minmax(from, to)) == edges.end())
		{
			edges[std::minmax(from, to)] = i + 1;
			g.addEdge(from - 1, to - 1);
		}
		else
		{
			edges[std::minmax(from, to)] = -1;
		}
	}
	std::vector<std::pair<size_t, size_t>> bridges = GraphProcessing::getBridges(g);
	size_t number_of_bridges = 0;
	if (bridges.size())
	{
		std::set<size_t> answer;
		for (auto i : bridges)
		{
			if (edges[i] != -1)
			{
				number_of_bridges++;
				answer.insert(edges[i]);
			}
		}
		std::cout << number_of_bridges << "\n";
		for (auto i : answer)
		{
			std::cout << i << " ";
		}
	}
	else
	{
		std::cout << 0;
	}
	return 0;
}