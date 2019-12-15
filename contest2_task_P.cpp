#include<iostream>
#include<string>
#include<vector>

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

int main()
{
	std::string str;
	std::cin >> str;
	std::vector<size_t> ans = StringProcessing::getPrefixFunction(str);
	for (size_t i : ans)
	{
		std::cout << i << " ";
	}

}