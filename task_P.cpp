#include<iostream>
#include<vector>
#include<algorithm>

class Edge
{
public:
	Edge(size_t from, size_t to, int weight)
	{
		from_ = from;
		to_ = to;
		weight_ = weight;
	}
	bool operator <(const Edge& second) const
	{
		return ((weight_ < second.weight_) /*|| (weight_ == second.weight_ && from_ < second.from_) || (weight_ == second.weight_ && from_ == second.from_ && to_ < second.to_)*/);
	}

	size_t getTo() const
	{
		return to_;
	}
	size_t getFrom() const
	{
		return from_;
	}
	size_t getWeight() const
	{
		return weight_;
	}
private:
	size_t from_, to_;
	int weight_;
};

class DSU
{
public:
	DSU(size_t n)
	{
		set_ = std::vector<size_t>(n);
		size_ = std::vector<size_t>(n, 1);
		sets_count_ = n;
		for (size_t i = 0; i < n; i++)
		{
			set_[i] = i;
		}
	}
	size_t findSet(size_t x)
	{
		if (x == set_[x])
		{
			return x;
		}
		set_[x] = findSet(set_[x]);
		return set_[x];
	}
	void Union(size_t x, size_t y)
	{
		x = findSet(x);
		y = findSet(y);
		if (x != y)
		{
			if (size_[x] == size_[y])
			{
				std::swap(x, y);
			}
			set_[y] = x;
			size_[x] += size_[y];
			sets_count_--;
		}
	}
	size_t getSetsCount() const
	{
		return sets_count_;
	}
private:
	std::vector<size_t> set_;
	std::vector<size_t> size_;
	size_t sets_count_;
};

namespace GraphProcessing
{
	std::vector<Edge> getMinimalSkeleton(std::vector<Edge>& edges, size_t VertexCount)
	{
		std::vector<Edge> skeleton;
		std::sort(edges.begin(), edges.end());
		DSU dsu(VertexCount);
		for (Edge edge : edges)
		{
			if (dsu.findSet(edge.getFrom()) != dsu.findSet(edge.getTo()))
			{
				skeleton.push_back(edge);
				dsu.Union(edge.getFrom(), edge.getTo());
			}
			if (dsu.getSetsCount() == 1)
			{
				break;
			}
		}
		return skeleton;
	}
}
int main()
{
	size_t n, m;
	std::cin >> n >> m;
	std::vector<Edge> edges;
	for (size_t i = 0; i < m; i++)
	{
		size_t to, from;
		int weight;
		std::cin >> from >> to >> weight;
		edges.push_back(Edge(from - 1, to - 1, weight));
	}
	
	int weight = 0;
	for (Edge edge : GraphProcessing::getMinimalSkeleton(edges, n))
	{
		weight += edge.getWeight();
	}
	std::cout << weight << "\n";
	return 0;
}
