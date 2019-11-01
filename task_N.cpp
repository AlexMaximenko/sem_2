#include<iostream>
#include<vector>
#include<map>
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

int main()
{
	size_t n, m;
	std::cin >> n >> m;
	DSU dsu(n);
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