#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<string.h>
namespace StringProcessing
{
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

std::string getEncryptedMessage(std::string& source, std::string& next_word)
{
	const size_t in_size = std::max(0, (int)(source.size() - next_word.size()));
	std::string current = std::move(source);
	std::vector<size_t> prefix = std::move(StringProcessing::getPrefixFunction(next_word + "#" + current.substr(in_size)));
	current += next_word.substr(prefix.back());
	return current;
}



int main()
{
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	size_t n;
	std::cin >> n;
	std::string current;
	std::cin >> current;
	for (size_t i = 0; i < n - 1; i++)
	{
		std::string new_word;
		std::cin >> new_word;
		current = std::move(getEncryptedMessage(current, new_word));
	}
	std::cout << current;
}
