#include<iostream>
#include<vector>
#include<unordered_map>
template<class T>
class DSU
{
public:
	DSU(const std::vector<T>& elements)
	{
		for (size_t i = 0; i < elements.size(); i++)
		{
			prev_[elements[i]] = elements[i];
			size_[elements[i]] = 1;
			set_weight_[elements[i]] = 0;
		}
		sets_count_ = elements.size();
	}
	T findSet(const T& x)
	{
		return x == prev_[x] ? x : prev_[x] = findSet(prev_[x]);
	}
	size_t getSetWeight(const T& x)
	{
		return x == prev_[x] ? set_weight_[x] : set_weight_[findSet(x)];
	}
	void Union(const T& x, const T& y, const size_t& weight = 0)
	{
		T prev_x = findSet(x);
		T prev_y = findSet(y);
		if (prev_x != prev_y)
		{
			if (size_[prev_x] == size_[prev_y])
			{
				std::swap(prev_x, prev_y);
			}
			set_weight_[prev_x] += set_weight_[prev_y] + weight;
			prev_[prev_y] = prev_x;
			size_[prev_x] += size_[prev_y];
			sets_count_--;
		}
		else
		{
			set_weight_[prev_x] += weight;
		}
	}
	size_t getSetsCount() const
	{
		return sets_count_;
	}
private:
	std::unordered_map<T, size_t> size_;
	std::unordered_map<T, T> prev_;
	size_t sets_count_;
	std::unordered_map<T, size_t> set_weight_;
};

int main()
{
	size_t n, m;
	std::scanf("%ld %ld", &n, &m);
	std::vector<size_t> elements;
	for (size_t i = 0; i < n; i++)
	{
		elements.push_back(i);
	}
	DSU<size_t> dsu(elements);
	for (size_t i = 0; i < m; i++)
	{
		size_t operation;
		scanf("%ld", &operation);
		if (operation == 2)
		{
			size_t vertex;
			scanf("%ld", &vertex);
			printf("%ld\n", dsu.getSetWeight(vertex - 1));
		}
		else
		{
			size_t to, from, weight;
			scanf("%ld %ld %ld", &to, &from, &weight);
			dsu.Union(to - 1, from - 1, weight);
		}
	}
	return 0;
}