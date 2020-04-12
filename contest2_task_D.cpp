#define  _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<math.h>
template<class T>
class SummFenwickTree
{
public:
	SummFenwickTree(size_t size)
	{
		elements_.resize(size, { 0,0 });
	}

	SummFenwickTree(const std::vector<T>& data)
	{
		elements_.resize(data.size(), { 0, 0 });
		for (size_t i = 0; i < data.size(); i++)
		{
			Update(i, data[i]);
		}
	}

	void Update(size_t num, T value)
	{
		while (num < elements_.size())
		{
			elements_[num].first += value;
			num = num | (num + 1);
		}
	}

	void AddAtSegment(size_t start, size_t finish, T value)
	{
		if (start == 0)
		{
			AddAtSegment_(finish, value);
			return;
		}
		if (start == finish)
		{
			Update(start, value);
			return;
		}
		AddAtSegment_(finish, value);
		AddAtSegment_(start - 1, -value);
	}

	T Query(size_t start, size_t finish)
	{
		if (start == 0)
		{
			return Sum_(finish);
		}
		return Sum_(finish) - Sum_(start - 1);
	}

private:
	T Sum_(size_t num)
	{
		T result = 0;
		size_t i = num;
		while (i >= 0)
		{
			result += elements_[i].first + elements_[i].second * (i - (i & (i + 1)) + 1);
			if (!(i & (i + 1)))
			{
				break;
			}
			i = (i & (i + 1)) - 1;
		}
		i = num | (num + 1);
		while (i < elements_.size())
		{
			result += elements_[i].second * (num - (i & (i + 1)) + 1);
			i = i | (i + 1);
		}
		return result;
	}

	void AddAtSegment_(size_t finish, T value)
	{
		size_t i = finish;
		while (i >= 0)
		{
			elements_[i].second += value;
			if ((i & (i + 1)) == 0)
			{
				break;
			}
			i = (i & (i + 1)) - 1;
		}
		i = finish | (finish + 1);
		while (i < elements_.size())
		{
			elements_[i].first += value * (finish - (i & (i + 1)) + 1);
			i = i | (i + 1);
		}
	}

	std::vector<std::pair<T, T>> elements_;
};


int main()
{
	int n;
	std::cin >> n;
	std::vector<int64_t> elements(n, 0);
	for (int i = 0; i < n; i++)
	{
		std::cin >> elements[i];
	}
	SummFenwickTree<int64_t> tree(elements);
	int m;
	char operation;
	std::cin >> m;
	int64_t start, finish, value, num;
	for (int i = 0; i < m; i++)
	{
		std::cin >> operation;
		if (operation == 'g')
		{
			std::cin >> num;
			std::cout << tree.Query(num - 1, num - 1) << std::endl;
		}
		else
		{
			std::cin >> start >> finish >> value;
			tree.AddAtSegment(start - 1, finish - 1, value);
		}
	}
}