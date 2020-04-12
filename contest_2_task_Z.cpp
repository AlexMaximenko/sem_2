#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<string.h>
namespace StringProcessing
{
	std::string getStrByZfunction(const std::vector<size_t>& z_function)
	{
		std::string result = "a";
		std::vector<size_t> symbols_in_block;
		bool is_last_in_block = true;
		size_t i = 1;
		while (i < z_function.size())
		{
			if (z_function[i] == 0)
			{
				if (is_last_in_block)
				{
					std::vector<bool> used_symbols(26, false);
					for (size_t k : symbols_in_block)
					{
						used_symbols[result[k] - 'a'] = true;
					}
					char curr_symbol = 'b';
					while (used_symbols[curr_symbol - 'a'])
					{
						curr_symbol++;
					}
					result += curr_symbol;
					is_last_in_block = false;
				}
				else
				{
					result += 'b';
				}

				i++;
			}
			else if (z_function[i] != 0)
			{
				size_t current_z_block = z_function[i];
				size_t curr_i = 0;
				symbols_in_block.clear();
				while (current_z_block > 0)
				{
					if (z_function[i] == current_z_block)
					{
						symbols_in_block.push_back(z_function[i]);
					}
					if (z_function[i] > current_z_block)
					{
						current_z_block = z_function[i];
						symbols_in_block.push_back(z_function[i]);
						curr_i = 0;
					}
					result += result[curr_i];
					curr_i++;
					i++;
					current_z_block--;
				}
				is_last_in_block = true;
			}
		}
		return result;
	}

}


int main()
{
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	size_t n;
	std::cin >> n;
	std::vector<size_t> z_function(n);
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> z_function[i];
	}
	std::cout << StringProcessing::getStrByZfunction(z_function);
}