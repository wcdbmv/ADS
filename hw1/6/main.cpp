/*
 * Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9]
 * размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать число,
 * которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
 * Напишите нерекурсивный алгоритм.
 * 
 * Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию
 * Partition реализуйте методом прохода двумя итераторами от конца массива к
 * началу.
 *
 * Требования к дополнительной памяти: O(n).
 * Требуемое среднее время работы: O(n).
 * 
 * Функцию Partition следует реализовывать методом прохода двумя итераторами в
 * одном направлении.
 * Описание для случая прохода от начала массива к концу:
 * - Выбирается опорный элемент. Опорный элемент меняется с последним элементом
 *   массива.
 * - Во время работы Partition в начале массива содержатся элементы, не бОльшие
 *   опорного. Затем располагаются элементы, строго бОльшие опорного. В конце
 *   массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
 * - Итератор (индекс) i указывает на начало группы элементов, строго бОльших
 *   опорного.
 * - Итератор j больше i, итератор j указывает на первый нерассмотренный
 *   элемент.
 * - Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он
 *   больше опорного, то сдвигаем j.
 * - Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и
 *   сдвигаем j.
 * - В конце работы алгоритма меняем опорный и элемент, на который указывает
 *   итератор i.
 */

#include <algorithm>
#include <cassert>
#include <iostream>

size_t MedianOfThree(int* array, size_t li, size_t ri) {
  assert(array);
  const size_t mi = li + (ri - li) / 2;
  if (array[ri] < array[li])
    std::swap(array[ri], array[li]);
  if (array[mi] < array[li])
    std::swap(array[mi], array[li]);
  if (array[ri] < array[mi])
    std::swap(array[ri], array[mi]);
  return mi;
}


int* Partition(int* begin, int* end) {
  if (begin == end)
    return begin;

  int* pivot = begin + MedianOfThree(begin, 0, end - begin);
  std::iter_swap(begin, pivot);

  int* pi = end - 1;
  for (int* pj = pi; pj != begin; --pj)
    if (*pj >= *begin)
      std::iter_swap(pi--, pj);
  std::iter_swap(begin, pi);

  return pi;
}

size_t Statistics(int* begin, int* end, size_t k) {
  int* left = begin;
  int* right = end;
  while (true) {
    int* pivot = Partition(left, right);
    size_t pivot_index = pivot - begin;
    if (pivot_index == k)
      return begin[k];
    if (pivot_index > k)
      right = pivot;
    else
      left = pivot + 1;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  size_t n = 0, k = 0;
  std::cin >> n >> k;
  assert(!std::cin.bad());

  int* array = new int[n];
  for (size_t i = 0; i != n; ++i)
    std::cin >> array[i];
  assert(!std::cin.bad());

  std::cout << Statistics(array, array + n - 1, k);

  delete[] array;
}
