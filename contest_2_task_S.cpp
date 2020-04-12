#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<string.h>
namespace StringProcessing
{
	size_t getMaxOfPrefixFunction(const std::string& str)
	{
		size_t max = 0;
		std::vector<size_t> prefix_function(str.length());
		prefix_function[0] = 0;
		for (size_t i = 1; i < str.length(); i++)
		{
			size_t k = prefix_function[i - 1];
			while (k > 0 && str[i] != str[k])
			{
				k = prefix_function[k - 1];
			}
			if (str[i] == str[k])
			{
				k++;
			}
			prefix_function[i] = k;
			if (k > max)
			{
				max = k;
			}
		}
		return max;
	}

	std::vector<size_t> getPrefixFunction(const std::string& str)
	{
		std::vector<size_t> prefix_function(str.length());
		prefix_function[0] = 0;
		for (size_t i = 1; i < str.length(); i++)
		{
			size_t k = prefix_function[i - 1];
			while (k > 0 && str[i] != str[k])
			{
				k = prefix_function[k - 1];
			}
			if (str[i] == str[k])
			{
				k++;
			}
			prefix_function[i] = k;
		}
		return prefix_function;
	}

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
	std::string str;
	size_t sub_count = 0;
	std::string curr, reversed_curr;
	std::cin >> str;
	for (size_t i = 0; i < str.size(); i++)
	{
		curr += str[i];
		reversed_curr = curr;
		std::reverse(reversed_curr.begin(), reversed_curr.end());
		sub_count += curr.size() - StringProcessing::getMaxOfPrefixFunction(reversed_curr);
	}
	std::cout << sub_count;
}