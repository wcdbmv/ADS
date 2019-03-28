/*
 * Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с
 * помощью динамического буфера.
 *
 * Формат входных данных.
 * В первой строке количество команд n. n ≤ 1000000.
 * Каждая команда задаётся как 2 целых числа: a b.
 * a = 2 - pop front
 * a = 3 - push back
 * Если дана команда pop*, то число b - ожидаемое значение. Если команда pop
 * вызвана для пустой структуры данных, то ожидается “-1”.
 *
 * Формат выходных данных.
 * Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если
 * хотя бы одно ожидание не оправдалось, то напечатать NO.
 */

#include <algorithm>
#include <cassert>
#include <iostream>

const int PUSH_COMMAND = 3;
const int POP_COMMAND = 2;
const int EMPTY_KEY = -1;

class Stack {
 public:
  explicit Stack(size_t size = DEFAULT_SIZE);
  ~Stack() { delete[] stack_; }

  void Push(int key);
  int Pop();

  bool Empty() const { return !fill_; }

  static const size_t DEFAULT_SIZE = 1024;

 private:
  int* stack_;
  size_t size_;
  size_t fill_;

  bool full() const { return fill_ == size_; }
  void grow();
};

Stack::Stack(size_t size) : size_(size), fill_(0) {
  assert(size_);
  stack_ = new int[size_];
}

void Stack::Push(int key) {
  if (full())
    grow();
  stack_[fill_++] = key;
}

int Stack::Pop() {
  assert(!Empty());
  return stack_[--fill_];
}

void Stack::grow() {
  size_t new_size = DEFAULT_SIZE;  // https://stackoverflow.com/a/5392048
  new_size = std::max(size_ << 1, new_size);
  int* new_stack = new int[new_size];
  for (size_t i = 0; i != fill_; ++i)
    new_stack[i] = stack_[i];
  size_ = new_size;
  stack_ = new_stack;
}

class Queue {
 public:
  explicit Queue(size_t stack_size = Stack::DEFAULT_SIZE);
  ~Queue();

  void Push(int key);
  int Pop();

  bool Empty() const { return left_stack_->Empty() && right_stack_->Empty(); }

 private:
  Stack* left_stack_;
  Stack* right_stack_;
};

Queue::Queue(size_t stack_size) {
  assert(stack_size);
  left_stack_ = new Stack(stack_size);
  right_stack_ = new Stack(stack_size);
}

Queue::~Queue() {
  delete left_stack_;
  delete right_stack_;
}

void Queue::Push(int key) { left_stack_->Push(key); }

int Queue::Pop() {
  assert(!Empty());
  if (right_stack_->Empty())
    while (!left_stack_->Empty())
      right_stack_->Push(left_stack_->Pop());
  return right_stack_->Pop();
}

bool Execute(int command, int key, Queue* queue) {
  assert(queue);
  switch (command) {
    case PUSH_COMMAND:
      queue->Push(key);
      return true;
    case POP_COMMAND:
      if (queue->Empty())
        return key == EMPTY_KEY;
      return queue->Pop() == key;
    default:
      assert(0);
  }
}

int ReadNum() {
  int num;
  std::cin >> num;
  assert(!std::cin.bad());
  return num;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  Queue queue;
  bool ok = true;
  int n = ReadNum();
  assert(n >= 0);
  for (int i = 0; i != n; ++i) {
    int command = ReadNum();
    assert(command == PUSH_COMMAND || command == POP_COMMAND);
    int key = ReadNum();
    ok &= Execute(command, key, &queue);
  }
  std::cout << (ok ? "YES" : "NO");
}
