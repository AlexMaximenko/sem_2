#include<iostream>
#include<vector>
#include<map>
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
		}
		sets_count_ = elements.size();
	}
	T findSet(const T& x)
	{
		return x == prev_[x] ? x : prev_[x] = findSet(prev_[x]);
	}
	void Union(const T& x, const T& y)
	{
		T prev_x = findSet(x);
		T prev_y = findSet(y);
		if (prev_x != prev_y)
		{
			if (size_[prev_x] == size_[prev_y])
			{
				std::swap(prev_x, prev_y);
			}
			prev_[prev_y] = prev_x;
			size_[prev_x] += size_[prev_y];
			sets_count_--;
		}
	}
	size_t getSetsCount() const
	{
		return sets_count_;
	}
private:
	std::map<T, size_t> size_;
	std::map<T, T> prev_;
	size_t sets_count_;
};

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	std::vector<size_t> elements;
	for (size_t i = 0; i < n; i++)
	{
		elements.push_back(i);
	}
	DSU<size_t> dsu(elements);
	for (size_t i = 0; i < m; i++)
	{
		size_t from, to;
		std::cin >> from >> to;
		dsu.Union(from, to);
		if (dsu.getSetsCount() == 1)
		{
			std::cout << i + 1;
			return 0;
		}
	}
	return 0;
}