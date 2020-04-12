#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
template<class T>
class MaxSegmentTree
{
public:
	MaxSegmentTree(const std::vector<T>& data)
	{
		size_t size = 1 << ((size_t)(log(data.size() - 1) / log(2) + 1));
		elements_.resize(2 * size - 1, { 0, 1 });
		for (size_t i = size - 1; i < size + data.size() - 1; i++)
		{
			elements_[i].first = data[i + 1 - size];
		}
		int i = size - 2;
		while (1)
		{
			elements_[i] = max_(elements_[2 * i + 1], elements_[2 * i + 2]);
			if (i != 0)
			{
				i--;
			}
			else
			{
				break;
			}
		}
	}

	MaxSegmentTree(size_t n)
	{
		size_t size = 1 << ((size_t)(log(n - 1) / log(2) + 1));
		elements_.resize(2 * size - 1, { 0, 0 });
	}
	std::pair<T, size_t> Query(size_t start, size_t finish, size_t node = 0, size_t curr_left = 0, size_t curr_right = 0)
	{
		if (node == 0)
		{
			curr_right = elements_.size() / 2;
		}
		if ((curr_left < start && curr_right < start) || (curr_left > finish && curr_right > finish))
		{
			return { 0, 0 };
		}
		if (start <= curr_left && finish >= curr_right)
		{
			return elements_[node];
		}

		return max_(Query(start, finish, (node + 1) * 2 - 1, curr_left, (curr_right + curr_left) / 2), Query(start, finish, (node + 1) * 2, (curr_right + curr_left) / 2 + 1, curr_right));
	}

private:
	std::pair<T, size_t> max_(const std::pair<T, size_t>& item_1, const std::pair<T, size_t>& item_2)
	{
		if (item_1.first > item_2.first)
		{
			return item_1;
		}
		if (item_1.first < item_2.first)
		{
			return item_2;
		}
		if (item_1.first == item_2.first)
		{
			std::pair<T, size_t> ans;
			ans.first = item_1.first;
			ans.second = item_1.second + item_2.second;
			return ans;
		}

	}

	std::vector<std::pair<T, size_t>> elements_;
};


int main()
{
	size_t n;
	std::cin >> n;
	std::vector<size_t> data(n);
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> data[i];
	}
	MaxSegmentTree<size_t> tree(data);
	size_t m;
	std::cin >> m;
	size_t start, finish;
	for (size_t i = 0; i < m; i++)
	{
		std::cin >> start >> finish;
		std::pair<size_t, size_t> query = tree.Query(start - 1, finish - 1);
		std::cout << query.first << " " << query.second << std::endl;
	}
}