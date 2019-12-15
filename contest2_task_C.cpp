#include<iostream>
#include<vector>
#include<math.h>
template<class T>
class NodSegmentTree
{
public:
	NodSegmentTree(const std::vector<T>& data)
	{
		size_t size = 1 << ((size_t)(log(data.size() - 1) / log(2) + 1));
		elements_.resize(2 * size - 1, 0);
		for (size_t i = size - 1; i < size + data.size() - 1; i++)
		{
			elements_[i] = data[i + 1 - size];
		}
		int i = size - 2;
		while (1)
		{
			elements_[i] = Nod(elements_[2 * i + 1], elements_[2 * i + 2]);
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

	NodSegmentTree(size_t n)
	{
		size_t size = 1 << ((size_t)(log(n - 1) / log(2) + 1));
		elements_.resize(2 * size - 1, 0);
	}
	T Query(size_t start, size_t finish, size_t node = 0, size_t curr_left = 0, size_t curr_right = 0)
	{
		if (node == 0)
		{
			curr_right = elements_.size() / 2;
		}
		if ((curr_left < start && curr_right < start) || (curr_left > finish && curr_right > finish))
		{
			return 0;
		}
		if (start <= curr_left && finish >= curr_right)
		{
			return elements_[node];
		}

		return Nod(Query(start, finish, (node + 1) * 2 - 1, curr_left, (curr_right + curr_left) / 2), Query(start, finish, (node + 1) * 2, (curr_right + curr_left) / 2 + 1, curr_right));
	}

	void print()
	{
		for (int i = 0; i < elements_.size(); i++)
		{
			std::cout << "element " << i << " = " << elements_[i] << std::endl;
		}
	}
private:
	T Nod(T first, T second)
	{
		while (second)
		{
			first %= second;
			std::swap(first, second);
		}
		return first;
	}
	std::vector<T> elements_;
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
	NodSegmentTree<size_t> tree(data);
	size_t m;
	std::cin >> m;
	size_t start, finish;
	for (int i = 0; i < m; i++)
	{
		std::cin >> start >> finish;
		std::cout << tree.Query(start - 1, finish - 1) << std::endl;
	}
}
