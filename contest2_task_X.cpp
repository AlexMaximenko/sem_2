#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<string.h>
namespace StringProcessing
{
	std::vector<size_t> getZfunction(const std::string& str)
	{
		std::vector<size_t> z_function(str.length());
		z_function[0] = z_function.size();
		size_t left = 0;
		size_t right = 0;
		for (size_t i = 1; i < str.length(); i++)
		{
			size_t j = (size_t)std::max(std::min((int)(right - i), (int)z_function[i - left]), 0);
			while (i + j < str.size() && str[j] == str[i + j])
			{
				j++;
			}
			if (i + j >= right)
			{
				left = i;
				right = i + j;
			}
			z_function[i] = j;
		}
		return z_function;
	}

	template<class T>
	std::vector<size_t> getZfunction(const std::vector<T>& str)
	{
		std::vector<size_t> z_function(str.size());
		z_function[0] = z_function.size();
		size_t left = 0;
		size_t right = 0;
		for (size_t i = 1; i < str.size(); i++)
		{
			size_t j = (size_t)std::max(std::min((int)(right - i), (int)z_function[i - left]), 0);
			while (i + j < str.size() && str[j] == str[i + j])
			{
				j++;
			}
			if (i + j >= right)
			{
				left = i;
				right = i + j;
			}
			z_function[i] = j;
		}
		return z_function;
	}

	std::string getReversedStr(const std::string& str)
	{
		std::string reversed_string = str;
		std::reverse(reversed_string.begin(), reversed_string.end());
		return reversed_string;
	}

	template<class T>
	std::vector<T> getReversedStr(const std::vector<T>& str)
	{
		std::vector<T> reversed_str = str;
		std::reverse(reversed_str.begin(), reversed_str.end());
		return reversed_str;
	}
}

template<class T>
std::vector<size_t> getCubesCount(const std::vector<T>& str)
{
	std::vector<T> curr_str;
	for (size_t i = 0; i < str.size(); i++)
	{
		curr_str.push_back(str[i]);
	}
	curr_str.push_back(-1);
	for (size_t i = 0; i < str.size(); i++)
	{
		curr_str.push_back(str[str.size() - i - 1]);
	}
	std::vector<size_t> z_function = StringProcessing::getZfunction(curr_str);
	std::vector<size_t> cubes_sizes = { str.size() };
	for (size_t i = str.size(); i < curr_str.size(); i++)
	{
		if (z_function[i] % 2 == 0 && z_function[i] + i == curr_str.size())
		{
			cubes_sizes.push_back(str.size() - z_function[i] / 2);
		}
	}
	std::sort(cubes_sizes.begin(), cubes_sizes.end());
	return cubes_sizes;

}

template<class T>
void printVector(std::vector<T> vect)
{
	for (T i : vect)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}


int main()
{
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	size_t n, m;
	std::cin >> n >> m;
	std::vector<size_t> str(n);
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> str[i];
	}
	printVector(getCubesCount(str));
}