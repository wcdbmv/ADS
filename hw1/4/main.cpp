/*
 * Вовочка ест фрукты из бабушкиной корзины. В корзине лежат фрукты разной
 * массы. Вовочка может поднять не более K грамм. Каждый фрукт весит не более K
 * грамм. За раз он выбирает несколько самых тяжелых фруктов, которые может
 * поднять одновременно, откусывает от каждого половину и кладет огрызки обратно
 * в корзину. Если фрукт весит нечетное число грамм, он откусывает большую
 * половину. Фрукт массы 1гр он съедает полностью. Определить за сколько
 * подходов Вовочка съест все фрукты в корзине.
 *
 * Формат входных данных. Вначале вводится n - количество фруктов и n строк с
 * массами фруктов. Затем K - "грузоподъемность".
 * Формат выходных данных. Неотрицательное число - количество подходов к
 * корзине.
 *
 * Решение всех задач данного раздела предполагает использование кучи,
 * реализованной в виде класса.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

class BinaryHeap {
 public:
  explicit BinaryHeap(const std::vector<int>& vector);
  ~BinaryHeap() {}

  void Push(int key);
  int Pop();

  bool Empty() const { return heap_.empty(); }

  int top() const;
 private:
  std::vector<int> heap_;

  void SiftUp(size_t i);
  void SiftDown(size_t i);
  void Heapify();
};

static inline __attribute__((always_inline)) size_t Parent(int t) {
  return (t - 1) / 2;
}

static inline __attribute__((always_inline)) size_t Child(size_t t) {
  return 2 * t + 1;
}

BinaryHeap::BinaryHeap(const std::vector<int>& vector) {
  heap_ = vector;
  Heapify();
}

void BinaryHeap::Push(int key) {
  heap_.push_back(key);
  SiftUp(heap_.size() - 1);
}

int BinaryHeap::Pop() {
  assert(!Empty());
  int max_key = heap_[0];
  heap_[0] = heap_[heap_.size() - 1];
  heap_.pop_back();
  SiftDown(0);
  return max_key;
}

int BinaryHeap::top() const {
  assert(!Empty());
  return heap_[0];
}

void BinaryHeap::SiftUp(size_t target) {
  while (heap_[target] > heap_[Parent(target)]) {
    std::swap(heap_[target], heap_[Parent(target)]);
    target = Parent(target);
  }
}

void BinaryHeap::SiftDown(size_t target) {
  while (Child(target) < heap_.size()) {
    const size_t l_child = Child(target);
    const size_t r_child = Child(target) + 1;
    size_t max_child = l_child;
    if (r_child < heap_.size() && heap_[r_child] > heap_[l_child])
      max_child = r_child;
    if (heap_[target] >= heap_[max_child])
      break;
    std::swap(heap_[max_child], heap_[target]);
    target = max_child;
  }
}

void BinaryHeap::Heapify() {
  for (int target = heap_.size() / 2 - 1; target >= 0; --target)
    SiftDown(target);
}

void HalveMaxSumK(BinaryHeap* heap, size_t k) {
  std::vector<int> halves;
  size_t sum = 0;
  while (!heap->Empty() && heap->top() + sum <= k) {
    sum += heap->top();
    halves.push_back(heap->Pop() / 2);
  }
  for (size_t i = 0; i != halves.size(); ++i)
    if (halves[i] > 0)
      heap->Push(halves[i]);
}

int ReadNum() {
  int num;
  std::cin >> num;
  assert(!std::cin.bad());
  return num;
}

void ReadArray(std::vector<int>* vector) {
  for (size_t i = 0; i != vector->size(); ++i) {
    (*vector)[i] = ReadNum();
    assert((*vector)[i] > 0);
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int n = ReadNum();
  assert(n > 0);

  std::vector<int> vector(n);
  ReadArray(&vector);

  int k = ReadNum();
  assert(k > 0);

  BinaryHeap heap(vector);

  size_t n_steps = 0;
  while (!heap.Empty()) {
    HalveMaxSumK(&heap, k);
    ++n_steps;
  }

  std::cout << n_steps;
}
