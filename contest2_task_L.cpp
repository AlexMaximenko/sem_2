#include<iostream>
#include<vector>
#include<random>
#include<ctime>
template<class T>
struct Node
{
	Node() = default;
	Node(const T& key) : key(key), priority(rand()), size(1), summ(key)
	{}
	T key;
	int priority;
	int size;
	T summ;
	Node* left = nullptr;
	Node* right = nullptr;
};


template<class T>
class Treap
{
private:

	//std::mt19937 random_generator_;

	mutable Node<T>* root_ = nullptr;

	/*int generatePriority()
	{
		return random_generator_();
	}*/

	T getSize(Node<T>* &current_node)
	{
		return (current_node == nullptr) ? 0 : current_node->size;
	}

	void resize(Node<T>* &current_node)
	{
		current_node->size = 1 + getSize(current_node->left) + getSize(current_node->right);
		current_node->summ = getSum(current_node->left) + getSum(current_node->right) + current_node->key;
	}

	Node<T>* merge(Node<T>* left_node, Node<T>* right_node)
	{
		if (left_node == nullptr || right_node == nullptr)
		{
			return (left_node != nullptr) ? left_node : right_node;
		}
		else
		{
			if (left_node->priority >= right_node->priority)
			{
				left_node->right = merge(left_node->right, right_node);
				resize(left_node);
				return left_node;
			}
			else
			{
				right_node->left = merge(left_node, right_node->left);
				resize(right_node);
				return right_node;
			}
		}
	}

	void split(Node<T>* current_node, const T& key, Node<T>* &left, Node<T>* &right)
	{
		if (current_node == nullptr)
		{
			left = right = nullptr;
			return;
		}
		else
		{
			if (key >= current_node->key)
			{
				split(current_node->right, key, current_node->right, right);
				left = current_node;
			}
			else
			{
				split(current_node->left, key, left, current_node->left);
				right = current_node;
			}
		}
		resize(current_node);
	}

public:
	Treap() = default;

	void insert(const T& key)
	{
		if (findElement(key, root_))
		{
			return;
		}
		Node<T> *left, *right;
		split(root_, key, left, right);
		Node<T>* new_node = new Node<T>(key);
		root_ = merge(merge(left, new_node), right);
	}

	bool findElement(const T& key) const
	{
		return findElement(key, root_);
	}

	bool findElement(const T& key, Node<T>* current_node) const
	{
		if (current_node == nullptr)
		{
			return false;
		}
		if (key == current_node->key)
		{
			return true;
		}
		else
		{
			if (key < current_node->key)
			{
				return findElement(key, current_node->left);
			}
			else
			{
				return findElement(key, current_node->right);
			}
		}
	}

	T getSum(Node<T>* &current_node)
	{
		return (current_node == nullptr) ? 0 : current_node->summ;
	}

	T sum(size_t start, size_t finish)
	{
		return sum(root_, start, finish);
	}

	T sum(Node<T>* &current_node, size_t start, size_t finish)
	{
		Node<T> *temp_left, *temp_right;
		Node<T> *left, *right;
		split(root_, finish, temp_left, temp_right);
		split(temp_left, start - 1, left, right);
		T result = getSum(right);
		root_ = merge(merge(left, right), temp_right);
		return result;
	}
};

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	Treap<int64_t> tree;
	size_t n;
	std::cin >> n;
	int64_t curr_summ;
	int request_number = 1;
	size_t start, finish;
	char cmd;
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> cmd;
		switch (cmd)
		{
		case '+':
		{
			std::cin >> start;
			if (request_number == 1)
			{
				tree.insert(start);
			}
			else {
				tree.insert((start + curr_summ) % 1000000000);
			}
			request_number = 1;
			break;
		}
		case '?':
		{
			std::cin >> start >> finish;
			curr_summ = tree.sum(start, finish);
			std::cout << curr_summ << '\n';
			request_number = 2;
			break;
		}
		}
	}
}