/*
 * Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
 * Найти количество таких пар индексов (i, j), что A[i] + B[i] = k. Время работы
 * O(n + m). n, m ≤ 100000. Указание: обходите массив B от конца к началу.
 */

#include <cassert>
#include <iostream>

int CountIndices(int k, const int* a, int n, const int* b, int m) {
  assert(a && n > 0 && b && m > 0);
  int i = 0, j = m - 1;
  int count = 0;
  while (i < n && j >= 0) {
    const int sum = a[i] + b[j];
    if (sum == k) {
      ++count;
      ++i;
      --j;
    } else if (sum < k) {
      ++i;
    } else {
      --j;
    }
  }
  return count;
}

int ReadNum() {
  int num;
  std::cin >> num;
  assert(!std::cin.bad());
  return num;
}

int* ReadArray(int size) {
  int* array = new int[size];
  for (int i = 0; i != size; ++i)
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

  int k = ReadNum();

  std::cout << CountIndices(k, a, n, b, m);

  DeleteArray(a);
  DeleteArray(b);
}
