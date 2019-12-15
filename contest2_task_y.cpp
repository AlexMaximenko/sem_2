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
void isLastWord(const std::string& source_word, const std::string& jacks_word_)
{
	std::string jacks_word = jacks_word_;
	std::string summ_str = source_word + "#" + jacks_word;
	std::vector<std::string> jacks_word_prefixes;
	std::vector<size_t> z_function = StringProcessing::getZfunction(summ_str);
	size_t prefixes_summ_size = 0;
	size_t curr_size = z_function.size();
	for (size_t i = z_function.size() - 1; i > source_word.size(); --i)
	{
		if (z_function[i] + i >= curr_size)
		{
			size_t curr_length = z_function[i] - curr_size + i;
			std::string curr_prefix;
			for (size_t j = i; j < curr_size; j++)
			{
				prefixes_summ_size++;
				curr_prefix += summ_str[j];
			}
			jacks_word_prefixes.push_back(curr_prefix);
			curr_size += curr_length - z_function[i];
		}
	}
	if (prefixes_summ_size == jacks_word_.length())
	{
		std::cout << "No\n";
		size_t i = jacks_word_prefixes.size() - 1;
		while (1)
		{
			std::cout << jacks_word_prefixes[i] << " ";
			if (i > 0)
			{
				i--;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "Yes\n";
	}
}

int main()
{
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	std::string source_word;
	std::string jacks_word;
	std::cin >> source_word;
	std::cin >> jacks_word;
	isLastWord(source_word, jacks_word);
	return 0;
}