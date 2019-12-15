#include<iostream>
#include<vector>
#include<string>

namespace StringProcessing
{
	std::string getStrByPrefix(const std::vector<size_t> & prefix_function)
	{
		if (!prefix_function.size())
		{
			return "";
		}
		std::string str = "a";

		for (size_t i = 1; i < prefix_function.size(); i++)
		{
			if (prefix_function[i] != 0)
			{
				str += str[prefix_function[i] - 1];
			}
			else
			{
				std::vector<bool> used(26, false);
				size_t k = prefix_function[i - 1];
				while (k > 0)
				{
					used[str[k] - 'a'] = true;
					k = prefix_function[k - 1];
				}
				char curr_symb = 'b';
				while (used[curr_symb - 'a'])
				{
					curr_symb++;
				};
				str += curr_symb;
			}
		}
		return str;
	}
}


int main()
{
	size_t n;
	std::cin >> n;
	std::vector<size_t> prefix_function(n);
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> prefix_function[i];
	}
	std::cout << StringProcessing::getStrByPrefix(prefix_function);

}