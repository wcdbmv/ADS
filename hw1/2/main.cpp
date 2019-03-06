/*
 * Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых
 * чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс
 * элемента массива A[k], ближайшего по значению к B[i]. Время работы поиска для
 * каждого элемента B[i]: O(log(k)). n ≤ 110000, m ≤ 1000.
 */

#include <cassert>
#include <iostream>

size_t LowerBound(const int* array, size_t n, int key) {
  assert(array && n);
  size_t li = 0, ri = n;
  while (li < ri) {
    const size_t mi = li + (ri - li) / 2;
    if (array[mi] < key)
      li = mi + 1;
    else
      ri = mi;
  }
  return li;
}

size_t FindNearest(const int* array, size_t n, int key) {
  assert(array && n);
  size_t i = LowerBound(array, n, key);
  if (i == 0)
    return i;
  if (i == n)
    return i - 1;
  const size_t left = key - array[i - 1];
  const size_t right = array[i] - key;
  return left <= right ? i - 1 : i;
}

int ReadNum() {
  int num;
  std::cin >> num;
  assert(!std::cin.bad());
  return num;
}

int* ReadArray(size_t size) {
  int* array = new int[size];
  for (size_t i = 0; i != size; ++i)
    array[i] = ReadNum();
  return array;
}

void DeleteArray(int* array) {
  delete[] array;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int n = ReadNum();
  assert(n > 0);
  int* a = ReadArray(n);

  int m = ReadNum();
  assert(m > 0);
  int* b = ReadArray(m);

  for (int i = 0; i != m; ++i)
    std::cout << FindNearest(a, n, b[i]) << ' ';

  DeleteArray(a);
  DeleteArray(b);
}
