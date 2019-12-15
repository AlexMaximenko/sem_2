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
	std::vector<size_t> getOccurences(const std::string& s, const std::string &t)
	{
		std::string ts = t + "#" + s;
		std::vector<size_t> occurences;
		std::vector<size_t> prefix_function = getPrefixFunction(ts);
		for (size_t i = t.length() - 1; i < ts.length(); i++)
		{
			if (prefix_function[i] == t.length())
			{
				occurences.push_back(i - t.length() * 2);
			}
		}
		return occurences;
	}
}

int main()
{
	std::string S, T;
	std::cin >> S;
	std::cin >> T;
	std::vector<size_t> ans = StringProcessing::getOccurences(S, T);
	for (size_t i : ans)
	{
		std::cout << i << std::endl;
	}

}