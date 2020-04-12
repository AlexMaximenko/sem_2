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
	bool isOccurence(const std::string& s, const std::string &t)
	{
		std::string ts = t + "#" + s;
		std::vector<size_t> prefix_function = getPrefixFunction(ts);
		for (size_t i = t.length() - 1; i < ts.length(); i++)
		{
			if (prefix_function[i] == t.length())
			{
				return true;
			}
		}
		return false;
	}
}
int isLoaded(const std::string& first, const std::string& second)
{
	bool is_loaded = false;
	bool is_empty = false;
	std::string first_1 = first + "1";
	std::string first_0 = first + "0";
	std::string second_1 = second + "1";
	std::string second_0 = second + "0";
	if (StringProcessing::isOccurence(first_1 + first_1, second_1) || StringProcessing::isOccurence(first_0 + first_0, second_1))
	{
		is_loaded = true;
	}
	if (StringProcessing::isOccurence(first_1 + first_1, second_0) || StringProcessing::isOccurence(first_0 + first_0, second_0))
	{
		is_empty = true;
	}
	if (is_empty && !is_loaded)
	{
		return 0;
	}
	if (!is_empty && is_loaded)
	{
		return 1;
	}
	return 2;
}

int main()
{
	std::string first, second;
	size_t n;
	std::cin >> n;
	std::cin >> first;
	std::cin >> second;
	int flag = isLoaded(first, second);
	if (flag == 0)
	{
		std::cout << "No";
	}
	else
	{
		if (flag == 1)
		{
			std::cout << "Yes";
		}
		else
		{
			std::cout << "Random";
		}
	}

}