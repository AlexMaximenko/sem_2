#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
class SparseTable
{
public:
	SparseTable(const std::vector<int>& elements)
	{
		int x_size = log(elements.size()) / log(2) + 1;
		table = std::vector<std::vector<int>>(x_size, std::vector<int>(elements.size()));
		for (int i = 0; i < elements.size(); i++)
		{
			table[0][i] = elements[i];
		}
		int pow = 1;
		for (int i = 1; i < x_size; i++)
		{
			pow *= 2;
			for (int j = 0; j < elements.size() - pow + 1; j++)
			{
				table[i][j] = std::min(table[i - 1][j], table[i - 1][j + pow / 2]);
			}
		}
		for (int i = 0; i <= elements.size(); i++)
		{
			log_table.push_back(log(i) / log(2));
		}
	}

	int GetMin(int start, int finish)
	{
		if (start > finish)
		{
			std::swap(start, finish);
		}
		int k = log_table[finish - start + 1];
		int pow = 1 << k;
		return std::min(table[k][start], table[k][finish - pow + 1]);
	}
private:
	std::vector<std::vector<int>> table;
	std::vector<int> log_table;
};
int main()
{
	int n, m, a1;
	std::cin >> n >> m >> a1;
	std::vector<int> elem(n);
	elem[0] = a1;
	for (int i = 1; i < n; i++)
	{
		elem[i] = (23 * elem[i - 1] + 21563) % 16714589;
	}
	SparseTable table(elem);
	int v_cur, u_cur, ans_cur;
	std::cin >> u_cur >> v_cur;
	ans_cur = table.GetMin(u_cur - 1, v_cur - 1);
	for (int i = 1; i < m; i++)
	{
		u_cur = (17 * u_cur + 751 + ans_cur + 2 * i) % n + 1;
		v_cur = (13 * v_cur + 593 + ans_cur + 5 * i) % n + 1;
		ans_cur = table.GetMin(u_cur - 1, v_cur - 1);
	}
	std::cout << u_cur << " " << v_cur << " " << ans_cur;
}