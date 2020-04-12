#include<iostream>
#include<vector>
const uint32_t N = 1 << 24;
const uint64_t MOD = 4294967296;

uint32_t a, b, cur = 0;
uint32_t nextRand()
{
	cur = cur * a + b;
	return cur >> 8;
}
int main()
{
	uint32_t m, q, l, add, r;
	std::cin >> m >> q;
	std::cin >> a >> b;
	std::vector<uint32_t> prefix_summ(N, 0);
	std::vector<int64_t> temp(N, 0);
	for (size_t i = 0; i < m; i++)
	{
		add = nextRand();
		l = nextRand();
		r = nextRand();
		if (l > r)
		{
			std::swap(l, r);
		}
		temp[l] += add;
		temp[l] %= MOD;
		temp[r + 1] -= add;
		temp[r + 1] %= MOD;
	}

	uint32_t add_curr = 0, summ = 0;
	for (size_t i = 0; i < N; i++)
	{
		add_curr += temp[i];
		add_curr %= MOD;
		summ += add_curr;
		summ %= MOD;
		prefix_summ[i] = summ;
	}
	uint32_t ans = 0;
	for (size_t i = 0; i < q; i++)
	{
		ans %= MOD;
		l = nextRand();
		r = nextRand();
		if (l > r)
		{
			std::swap(l, r);
		}
		if (l != 0)
		{
			ans += (prefix_summ[r] - prefix_summ[l - 1]);
		}
		else
		{
			ans += prefix_summ[r];
		}
	}
	std::cout << ans;
	return 0;
}
