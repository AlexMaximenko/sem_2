#include<iostream>
#include<vector>
#include<random>
#include<ctime>
#include<string>
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

	mutable Node<T>* root_ = nullptr;

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

	void deleteElement(const T& key)
	{
		deleteElement(root_, key);
	}

	void deleteElement(Node<T>* &current_node, const T& key)
	{
		if (findElement(key, root_))
		{
			Node<T>* left;
			Node<T>* right;
			Node<T>* left_left;
			Node<T>* left_right;
			split(current_node, key, left, right);
			split(left, key - 1, left_left, left_right);
			current_node = merge(left_left, right);
		}
	}

	void next(const T& key)
	{
		next(root_, key);
	}

	void prev(const T& key)
	{
		prev(root_, key);
	}

	void kOrd(const size_t& k)
	{
		kOrd(root_, k);
	}

	void next(Node<T>* &current_node, const T& key)
	{
		Node<T> *left, *right;
		Node<T>* tmp;
		split(current_node, key, left, right);
		if (right == nullptr)
		{
			std::cout << "none\n";
			return;
		}
		tmp = right;
		while (tmp->left != nullptr)
		{
			tmp = tmp->left;
		}
		std::cout << tmp->key << "\n";
		current_node = merge(left, right);
	}

	void prev(Node<T>* &current_node, const T& key)
	{
		Node<T> *left, *right;
		Node<T>* tmp;
		split(current_node, key - 1, left, right);
		if (left == nullptr)
		{
			std::cout << "none\n";
			return;
		}
		tmp = left;
		while (tmp->right != nullptr)
		{
			tmp = tmp->right;
		}
		std::cout << tmp->key << "\n";
		current_node = merge(left, right);
	}

	bool kOrd(Node<T>* &current_node, const size_t& k)
	{
		if (current_node == nullptr)
		{
			std::cout << "none\n";
			return false;
		}
		if (getSize(current_node->left) == k - 1)
		{
			std::cout << current_node->key << "\n";
			return true;
		}
		if (getSize(current_node->left) < k - 1)
		{
			return kOrd(current_node->right, k - getSize(current_node->left) - 1);
		}
		else
		{
			return kOrd(current_node->left, k);
		}
	}
};

int main()
{
	Treap<int64_t> tree;
	std::string cmd;
	int value;
	while (std::cin >> cmd)
	{
		std::cin >> value;
		if (cmd == "insert")
		{
			tree.insert(value);
		}
		if (cmd == "delete")
		{
			tree.deleteElement(value);
		}
		if (cmd == "exists")
		{
			if (tree.findElement(value))
			{
				std::cout << "true\n";
			}
			else
			{
				std::cout << "false\n";
			}
		}
		if (cmd == "next")
		{
			tree.next(value);
		}
		if (cmd == "prev")
		{
			tree.prev(value);
		}
		if (cmd == "kth")
		{
			tree.kOrd(value);
		}
	}
}