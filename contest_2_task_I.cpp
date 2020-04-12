#include<iostream>
#include<vector>
#include<math.h>
template<class T>
class SummFenwick_3_Tree
{
public:
	SummFenwick_3_Tree(size_t size_x, size_t size_y, size_t size_z)
	{
		elements_ = std::vector<std::vector<std::vector<T>>>(size_x, std::vector<std::vector<T>>(size_y, std::vector<T>(size_z, 0)));
	}
	SummFenwick_3_Tree(const std::vector<std::vector<std::vector<T>>>& data)
	{
		elements_ = std::vector<std::vector<std::vector<T>>>(data.size(), std::vector<std::vector<T>>(data[0].size(), std::vector<T>(data[0][0].size(), 0)));
		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data[0].size(); j++)
			{
				for (size_t k = 0; k < data[0][0].size(); k++)
				{
					Update(i, j, k, data[i][j][k]);
				}
			}
		}
	}

	void Update(size_t num_x, size_t num_y, size_t num_z, T value)
	{
		for (size_t i = num_x; i < elements_.size(); i |= i + 1)
		{
			for (size_t j = num_y; j < elements_[0].size(); j |= j + 1)
			{
				for (size_t k = num_z; k < elements_[0][0].size(); k |= k + 1)
				{
					elements_[i][j][k] += value;
				}
			}
		}
	}

	T GetSum(size_t start_x, size_t start_y, size_t start_z, size_t finish_x, size_t finish_y, size_t finish_z)
	{
		T s1 = Sum(finish_x, finish_y, finish_z) - Sum(start_x - 1, finish_y, finish_z) - Sum(finish_x, start_y - 1, finish_z) - Sum(finish_x, finish_y, start_z - 1);
		T s2 = Sum(start_x - 1, start_y - 1, finish_z) + Sum(start_x - 1, finish_y, start_z - 1) + Sum(finish_x, start_y - 1, start_z - 1);
		return (s1 + s2 - Sum(start_x - 1, start_y - 1, start_z - 1));
	}

private:
	T Sum(size_t num_x, size_t num_y, size_t num_z)
	{
		if (num_x == -1 || num_y == -1 || num_z == -1)
		{
			return 0;
		}
		T result = 0;
		size_t i = num_x;
		size_t j = num_y;
		size_t k = num_z;
		while (i >= 0)
		{
			j = num_y;
			while (j >= 0)
			{
				k = num_z;
				while (k >= 0)
				{
					result += elements_[i][j][k];
					if (!(k & (k + 1)))
					{
						break;
					}
					k = (k & (k + 1)) - 1;
				}
				if (!(j & (j + 1)))
				{
					break;
				}
				j = (j & (j + 1)) - 1;
			}
			if (!(i & (i + 1)))
			{
				break;
			}
			i = (i & (i + 1)) - 1;
		}
		return result;
	}
	std::vector<std::vector<std::vector<T>>> elements_;
};

int main()
{
	size_t n;
	std::cin >> n;
	SummFenwick_3_Tree<int64_t> tree(n, n, n);
	size_t m = 0;
	while (m != 3)
	{
		std::cin >> m;
		if (m == 1)
		{
			size_t x, y, z;
			int k;
			std::cin >> x >> y >> z >> k;
			tree.Update(x, y, z, k);
		}
		if (m == 2)
		{
			size_t x1, y1, z1, x2, y2, z2;
			std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
			std::cout << tree.GetSum(x1, y1, z1, x2, y2, z2) << std::endl;
		}
	}
}