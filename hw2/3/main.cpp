/*
 * Задача 3. Декартово дерево
 *
 * Дано число N < 10^6 и последовательность пар целых чисел из [-2^31..2^31]
 * длиной N. Построить декартово дерево из N узлов, характеризующихся парами
 * чисел {Xi, Yi}. Каждая пара чисел {Xi, Yi} определяет ключ Xi и приоритет Yi
 * декартовом дереве. Добавление узла в декартово дерево выполняйте второй
 * версией алгоритма, рассказанного на лекции:
 *   При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим
 *   приоритетом. Затем разбейте найденное поддерево по ключу x так, чтобы в
 *   первом поддереве все ключи меньше x, а во втором больше или равны x.
 *   Получившиеся два дерева сделайте дочерними для нового узла (x, y).
 *   Новый узел вставьте на место узла P.
 * Построить также наивное дерево поиска по ключам Xi методом из задачи 2.
 *
 * Вычислить количество узлов в самом широком слое декартового дерева и
 * количество узлов в самом широком слое наивного дерева поиска. Вывести их
 * разницу. Разница может быть отрицательна.
 */

#include <iostream>
#include <stack>
#include <queue>

template <typename T>
struct BaseBinaryTreeNode {
  explicit BaseBinaryTreeNode(const T& key);
  ~BaseBinaryTreeNode();

  T key;
  BaseBinaryTreeNode* left;
  BaseBinaryTreeNode* right;
};

template <typename T, typename NodeT = BaseBinaryTreeNode<T>>
class BaseBinaryTree {
 public:
  BaseBinaryTree();
  ~BaseBinaryTree();

  size_t MaxWidth() const;

 protected:
  NodeT* root_;
};

template <typename T>
class BinaryTree : public BaseBinaryTree<T> {
 public:
  void Insert(T key);
};

template <typename T>
struct TreapNode : BaseBinaryTreeNode<T> {
  explicit TreapNode(const T& key, int priority = 0);

  int priority;
  TreapNode<T>* left;
  TreapNode<T>* right;
};

template <typename T>
class Treap : public BaseBinaryTree<T, TreapNode<T>> {
 public:
  void Insert(T key, int priority);

 private:
  void Split(TreapNode<T>* curr, T key, TreapNode<T>** left, TreapNode<T>** right);
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  BinaryTree<int> binary_tree;
  Treap<int> treap;
  size_t n;
  std::cin >> n;
  for (size_t i = 0; i != n; ++i) {
    int key, priority;
    std::cin >> key >> priority;
    binary_tree.Insert(key);
    treap.Insert(key, priority);
  }
  std::cout << static_cast<int>(treap.MaxWidth())
             - static_cast<int>(binary_tree.MaxWidth()) << std::endl;
}

//——————————————————————————————————————————————————————————————————————————————

template <typename T>
BaseBinaryTreeNode<T>::BaseBinaryTreeNode(const T& key)
    : key(key), left(nullptr), right(nullptr) {}

template <typename T>
BaseBinaryTreeNode<T>::~BaseBinaryTreeNode() { delete left; delete right; }

template <typename T, typename NodeT>
BaseBinaryTree<T, NodeT>::BaseBinaryTree() : root_(nullptr) {}

template <typename T, typename NodeT>
BaseBinaryTree<T, NodeT>::~BaseBinaryTree() { delete root_; }

template <typename T>
TreapNode<T>::TreapNode(const T& key, int priority)
    : BaseBinaryTreeNode<T>(key), priority(priority) {}

template <typename T, typename NodeT>
size_t BaseBinaryTree<T, NodeT>::MaxWidth() const {
  if (!root_)
    return 0;
  size_t max_width = 0;
  std::queue<NodeT*> layer;
  layer.push(root_);
  while (!layer.empty()) {
    const size_t curr_width = layer.size();
    if (curr_width > max_width)
      max_width = curr_width;
    for (size_t i = 0; i != curr_width; ++i) {
      auto* node = layer.front();
      layer.pop();
      if (node->left)
        layer.push(node->left);
      if (node->right)
       layer.push(node->right);
    }
  }
  return max_width;
}

template <typename T>
void BinaryTree<T>::Insert(T key) {
  auto* node = new BaseBinaryTreeNode<T>(key);
  if (!BinaryTree<T>::root_) {
    BinaryTree<T>::root_ = node;
    return;
  }
  auto next = BinaryTree<T>::root_;
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
void Treap<T>::Insert(T key, int priority) {
  auto* node = new TreapNode<T>(key, priority);
  if (!Treap<T>::root_) {
    Treap<T>::root_ = node;
    return;
  }
  TreapNode<T>* curr = Treap<T>::root_;
  TreapNode<T>* prev = nullptr;
  while (curr && curr->priority >= priority) {
    prev = curr;
    curr = key <= curr->key ? curr->left : curr->right;
  }
  Split(curr, key, &node->left, &node->right);
  if (!prev)
    Treap<T>::root_ = node;
  else if (key <= prev->key)
    prev->left = node;
  else
    prev->right = node;
}

template <typename T>
void Treap<T>::Split(TreapNode<T>* curr, T key, TreapNode<T>** left, TreapNode<T>** right) {
  if (!curr) {
    *left = *right = nullptr;
  } else if (curr->key <= key) {
    Split(curr->right, key, &curr->right, right);
    *left = curr;
  } else {
    Split(curr->left, key, left, &curr->left);
    *right = curr;
  }
}
