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

	std::string getReversedStr(const std::string& str)
	{
		std::string reversed_string = str;
		std::reverse(reversed_string.begin(), reversed_string.end());
		return reversed_string;
	}

	std::vector<size_t> getAfunction(const std::string& str)
	{
		std::string buff = str + "#" + getReversedStr(str);
		std::vector<size_t> z_function = getZfunction(buff);
		std::vector<size_t> ans;
		for (size_t i = 0; i < str.length(); i++)
		{
			ans.push_back(z_function[z_function.size() - i - 1]);
		}
		return ans;
	}
}


int main()
{
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	std::string str;
	size_t n;
	std::cin >> n;
	std::cin >> str;
	std::vector<size_t> a_function = StringProcessing::getAfunction(str);
	for (auto i : a_function)
	{
		printf("%d ", i);
	}
}