#include<iostream>
#include<vector>
#include<random>
#include<ctime>
template<class T>
struct Node
{
	Node() = default;
	Node(const T& key) : key(key), priority(rand()), summ(key)
	{}
	T key;
	int priority;
	int size = 0;
	T summ = 0;
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
		return current_node == nullptr ? 0 : current_node->summ;
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

	int64_t getSum(Node<T>* &current_node) 
	{
		return (current_node == nullptr) ? 0 : current_node->sum;
	}

	int64_t sum(size_t start, size_t finish)
	{
		return sum(root_, start, finish);
	}

	int64_t sum(Node<T>* &current_node, size_t start, size_t finish)
	{
		Node<T> *temp_left, *temp_right;
		Node<T> *left, *right;
		split(root_, finish, temp_left, temp_right);
		split(temp_left, start - 1, left, right);
		int64_t result = get_sum(right);
		root_ = merge(merge(left, right), temp_right);
		return result;
	}
};

int main() {
	Treap<int64_t> tree;
	char cmd;
	int n, x, y;
	long long sum_x_y;
	int request_number = 1;
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cin >> n;
	for (int i = 0; i < n; ++i) {
		std::cin >> cmd;
		switch (cmd) {
		case '+':
		{
			std::cin >> x;
			if (request_number == 1) {
				tree.insert(x);
			}
			else {
				tree.insert((x + sum_x_y) % 1000000000);
			}
			request_number = 1;
			break;
		}
		case '?':
		{
			std::cin >> x >> y;
			sum_x_y = tree.sum(x, y);
			std::cout << sum_x_y << '\n';
			request_number = 2;
			break;
		}
		}
	}
}