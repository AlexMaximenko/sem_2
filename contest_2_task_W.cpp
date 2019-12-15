#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<string.h>
namespace StringProcessing
{
	std::string getStrByZfunction(const std::vector<size_t> & z_function)
	{
		if (z_function.size() == 0)
		{
			return "";
		}
		std::string alphabet = "abcdefghijklmnopqrtsuvwxyz";
		std::string result = "a";
		size_t current_prefix = 0;
		size_t position;
		size_t new_symbol = 1;
		for (size_t i = 1; i < z_function.size(); i++)
		{
			if (z_function[i] == 0 && current_prefix == 0)
			{
				result += alphabet[new_symbol++];
			}
			else
			{
				if (z_function[i] > current_prefix)
				{
					current_prefix = z_function[i];
					position = 0;
				}
				if (current_prefix > 0)
				{
					result += result[position++];
					current_prefix--;
				}
			}
		}
		return result;
	}

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
}


int main()
{
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	std::string str;
	std::cin >> str;
	std::vector<size_t> z_function = StringProcessing::getZfunction(str);
	for (auto i : z_function)
	{
		printf("%d ", i);
	}
}