#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
#include<string>
template<class T>
class BracketsSegmentTree
{
public:
	BracketsSegmentTree(const std::string& data)
	{
		size_t size;
		if (data.size() == 1)
		{
			size = 2;
		}
		else
		{
			size = 1 << ((size_t)(log(data.size() - 1) / log(2) + 1));
		}
		elements_.resize(2 * size - 1, Vertex());
		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i] == '(')
			{
				elements_[i + elements_.size() / 2].left = 1;
			}
			else if (data[i] == ')')
			{
				elements_[i + elements_.size() / 2].right = 1;
			}
		}
		size_t i = size - 2;
		while (1)
		{
			Vertex Left = elements_[2 * i + 1];
			Vertex Right = elements_[2 * i + 2];
			elements_[i].left = Left.left + Right.left - std::min(Left.left, Right.right);
			elements_[i].right = Left.right + Right.right - std::min(Left.left, Right.right);
			elements_[i].size = Left.size + Right.size + 2 * std::min(Left.left, Right.right);
			if (i != 0)
			{
				i--;
			}
			else
			{
				break;
			}

		}
	}

	T Query(size_t start, size_t finish) const
	{
		start += elements_.size() / 2;
		finish += elements_.size() / 2;
		Vertex result_left, result_right;
		while (start <= finish)
		{
			if (start % 2 == 0)
			{
				result_left.size += elements_[start].size + 2 * std::min(result_left.left, elements_[start].right);
				result_left.left -= std::min(result_left.left, elements_[start].right);
				result_left.left += elements_[start].left;
				start++;
			}
			if (finish % 2 == 1)
			{
				result_right.size += elements_[finish].size + 2 * std::min(result_right.right, elements_[finish].left);
				result_right.right -= std::min(result_right.right, elements_[finish].left);
				result_right.right += elements_[finish].right;
				finish--;
			}
			if (start > finish)
			{
				break;
			}
			start = (start - 1) / 2;
			finish = (finish - 1) / 2;
		}
		T result = result_left.size + result_right.size + 2 * std::min(result_right.right, result_left.left);
		return result;
	}

private:
	struct Vertex
	{
		Vertex() : left(0), right(0), size(0)
		{}
		T left, right;
		size_t size;
	};
	std::vector<Vertex> elements_;
};


int main()
{
	std::string data;
	std::cin >> data;
	BracketsSegmentTree<uint32_t> tree(data);
	uint32_t m, start, finish;
	std::cin >> m;
	for (size_t i = 0; i < m; i++)
	{
		std::cin >> start >> finish;
		std::cout << tree.Query(start - 1, finish - 1) << std::endl;
	}
	return 0;
}