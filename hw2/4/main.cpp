/*
 * Задача 4. Использование АВЛ-дерева
 *
 * Порядковые статистики. Дано число N и N строк. Каждая строка содержит команду
 * добавления или удаления натуральных чисел, а также запрос на получение k-ой
 * порядковой статистики. Команда добавления числа A задается положительным
 * числом A, команда удаления числа A задается отрицательным числом “-A”. Запрос
 * на получение k-ой порядковой статистики задается числом k. Требуемая скорость
 * выполнения запроса - O(log n).
 */

#include <iostream>
#include <cassert>

template <typename T>
class AvlTree {
 public:
  void Insert(const T& key);
  void Remove(const T& key);

  const T& Statistics(size_t k) const;

 protected:
  struct Node {
    explicit Node(const T& key);

    int BalanceFactor() const;
    void UpdateFields();

    Node* RotateLeft();
    Node* RotateRight();
    Node* Balance();

    Node* FindMin();
    Node* RemoveMin();

    static uint8_t HeightOf(const Node *node);
    static size_t CountOf(const Node *node);

    static Node* Insert(Node* node, const T& key);
    static Node* Remove(Node* node, const T& key);

    T key;
    Node* left = nullptr;
    Node* right = nullptr;

    uint8_t height = 1;
    size_t count = 1;
  };

  Node* root_ = nullptr;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t n;
  std::cin >> n;

  AvlTree<int> tree;

  for (size_t i = 0; i != n; ++i) {
    int key, k;
    std::cin >> key >> k;
    if (key < 0)
      tree.Remove(-key);
    else
      tree.Insert(key);
    std::cout << tree.Statistics(k) << std::endl;
  }
}

//——————————————————————————————————————————————————————————————————————————————

template <typename T>
AvlTree<T>::Node::Node(const T& key) : key(key) {}

template <typename T>
uint8_t AvlTree<T>::Node::HeightOf(const Node *node) {
  return node ? node->height : 0;
}

template <typename T>
size_t AvlTree<T>::Node::CountOf(const Node *node) {
  return node ? node->count : 0;
}

template <typename T>
int AvlTree<T>::Node::BalanceFactor() const {
  return static_cast<int>(HeightOf(right)) - HeightOf(left);
}

template <typename T>
void AvlTree<T>::Node::UpdateFields() {
  count = CountOf(left) + CountOf(right) + 1;
  height = std::max(HeightOf(left), HeightOf(right)) + 1;
}

template <typename T>
typename AvlTree<T>::Node* AvlTree<T>::Node::RotateLeft() {
  Node* node = right;
  right = right->left;
  node->left = this;

  this->UpdateFields();
  node->UpdateFields();

  return node;
}

template <typename T>
typename AvlTree<T>::Node* AvlTree<T>::Node::RotateRight() {
  Node* node = left;
  left = left->right;
  node->right = this;

  this->UpdateFields();
  node->UpdateFields();

  return node;
}

template <typename T>
typename AvlTree<T>::Node* AvlTree<T>::Node::Balance() {
  UpdateFields();

  if (BalanceFactor() == 2) {
    if (right->BalanceFactor() < 0)
      right = right->RotateRight();
    return RotateLeft();
  }
  if (BalanceFactor() == -2) {
    if (left->BalanceFactor() > 0)
      left = left->RotateLeft();
    return RotateRight();
  }

  return this;
}

template <typename T>
typename AvlTree<T>::Node* AvlTree<T>::Node::Insert(Node* node, const T& key) {
  if (!node)
    return new Node(key);
  if (key < node->key)
    node->left = Insert(node->left, key);
  else
    node->right = Insert(node->right, key);
  return node->Balance();
}

template <typename T>
typename AvlTree<T>::Node* AvlTree<T>::Node::FindMin() {
  return left ? left->FindMin() : this;
}

template <typename T>
typename AvlTree<T>::Node* AvlTree<T>::Node::RemoveMin() {
  if (!left)
    return right;
  left = left->RemoveMin();
  return Balance();
}

template <typename T>
typename AvlTree<T>::Node* AvlTree<T>::Node::Remove(Node* node, const T& key) {
  if (!node)
    return nullptr;
  if (key == node->key) {
    Node* left = node->left;
    Node* right = node->right;
    delete node;
    if (!right)
      return left;
    node = right->FindMin();
    node->right = right->RemoveMin();
    node->left = left;
  } else if (key < node->key)
    node->left = Remove(node->left, key);
  else
    node->right = Remove(node->right, key);
  return node->Balance();
}

template <typename T>
void AvlTree<T>::Insert(const T& key) { root_ = Node::Insert(root_, key); }

template <typename T>
void AvlTree<T>::Remove(const T& key) { root_ = Node::Remove(root_, key); }

template <typename T>
const T& AvlTree<T>::Statistics(size_t k) const {
  if (k >= Node::CountOf(root_))
    assert(0);
  Node* node = root_;
  int index = Node::CountOf(root_->left);
  while (index != k) {
    if (k > index) {
      node = node->right;
      index += Node::CountOf(node->left) + 1;
    } else {
      node = node->left;
      index -= Node::CountOf(node->right) + 1;
    }
  }
  return node->key;
}