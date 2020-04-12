#include<iostream>
#include<vector>
#include<math.h>
template<class T>
class SummFenwickTree
{
public:
	SummFenwickTree(size_t size)
	{
		elements_.resize(size, 0);
	}
	SummFenwickTree(const std::vector<T>& data)
	{
		elements_.resize(data.size(), 0);
		for (size_t i = 0; i < data.size(); i++)
		{
			Update(i, data[i]);
		}
	}

	void Update(size_t num, T value)
	{
		while (num < elements_.size())
		{
			elements_[num] += value;
			num = num | (num + 1);
		}
	}

	T GetSum(size_t start, size_t finish)
	{
		if (start == 0)
		{
			return Sum(finish);
		}
		return Sum(finish) - Sum(start - 1);
	}
private:
	T Sum(size_t num)
	{
		T result = 0;
		while (num >= 0)
		{
			result += elements_[num];
			if (num == 0)
			{
				break;
			}
			if (!(num & (num + 1)))
			{
				break;
			}
			num = (num & (num + 1)) - 1;
		}
		return result;
	}
	std::vector<T> elements_;
};

int main()
{
	size_t n;
	std::cin >> n;
	std::vector<int64_t> data(n);
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> data[i];
	}
	SummFenwickTree<int64_t> tree(data);
	size_t m;
	std::cin >> m;
	char operation;
	size_t start, finish;
	size_t num, value;
	for (size_t i = 0; i < m; i++)
	{
		std::cin >> operation;
		if (operation == 's')
		{
			std::cin >> start >> finish;
			std::cout << tree.GetSum(start - 1, finish - 1) << " ";
		}
		else
		{
			std::cin >> num >> value;
			value = value - data[num - 1];
			data[num - 1] += value;
			tree.Update(num - 1, value);
		}
	}
}