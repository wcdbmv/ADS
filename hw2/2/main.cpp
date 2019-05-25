/*
 * Задача 2. Порядок обхода
 *
 * Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной
 * N. Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т. е., при добавлении очередного числа K в дерево с корнем root, если
 * root → Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое
 * поддерево root.
 * Рекурсия запрещена.
 *
 * Выведите элементы в порядке in-order (слева направо).
 */

#include <iostream>
#include <functional>
#include <stack>

template <typename T>
class BinaryTree {
 public:
  struct Node {
    explicit Node(const T& key);
    ~Node();

    T key;
    Node* left;
    Node* right;
  };

  BinaryTree();
  ~BinaryTree();

  void Insert(T key);
  void TraverseInOrder(std::function<void(const Node*)> func) const;

 protected:
  Node* root_;
};

int main() {
  size_t n = 0;
  std::cin >> n;

  BinaryTree<int> tree;

  for (size_t i = 0; i != n; ++i) {
    int tmp;
    std::cin >> tmp;
    tree.Insert(tmp);
  }

  tree.TraverseInOrder([](const typename BinaryTree<int>::Node* node) {
    std::cout << node->key << ' ';
  });
}

//——————————————————————————————————————————————————————————————————————————————

template <typename T>
BinaryTree<T>::Node::Node(const T& key) : key(key), left(nullptr), right(nullptr) {}

template <typename T>
BinaryTree<T>::Node::~Node() { delete left; delete right; }

template <typename T>
BinaryTree<T>::BinaryTree() : root_(nullptr) {}

template <typename T>
BinaryTree<T>::~BinaryTree() { delete root_; }

template <typename T>
void BinaryTree<T>::Insert(T key) {
  Node* node = new Node(key);
  if (!root_) {
    root_ = node;
    return;
  }

  Node* next = root_;
  while (true) {
    if (key < next->key) {
      if (next->left) {
        next = next->left;
      } else {
        next->left = node;
        return;
      }
    } else {
      if (next->right) {
        next = next->right;
      } else {
        next->right = node;
        return;
      }
    }
  }
}

template <typename T>
void BinaryTree<T>::TraverseInOrder(std::function<void(const Node*)> func) const {
  std::stack<Node*> stack;
  Node* node = root_;
  while (node || !stack.empty()) {
    for (; node; node = node->left)
      stack.push(node);
    node = stack.top();
    stack.pop();
    func(node);
    node = node->right;
  }
}
