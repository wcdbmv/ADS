/*
 * LSD для long long.
 * Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше
 * 10^6. Отсортировать массив методом поразрядной сортировки LSD по байтам.
 */

#include <cassert>
#include <cstring>
#include <iostream>

typedef int64_t ull;

static const size_t INIT_ZEROS = 0;
static const size_t BYTE = 256;
static const size_t BITS_IN_BYTE = 8;

static inline __attribute__((always_inline)) int get_byte(ull num, size_t i) {
  return num >> (BITS_IN_BYTE * i) & (BYTE - 1);
}

void CountingSort(ull* array, size_t size, size_t i) {
  assert(array);
  size_t count[BYTE] = {INIT_ZEROS};
  for (size_t j = 0; j != size; ++j)
    ++count[get_byte(array[j], i)];
  for (size_t j = 1; j != BYTE; ++j)
    count[j] += count[j - 1];

  ull* res = new ull[size];
  for (int j = static_cast<int>(size) - 1; j >= 0; --j)
    res[--count[get_byte(array[j], i)]] = array[j];
  memcpy(array, res, size * sizeof (ull));
  delete[] res;
}

void LSDSort(ull* array, size_t size) {
  assert(array);
  for (size_t i = 0; i != sizeof (ull); ++i)
    CountingSort(array, size, i);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  size_t size;
  std::cin >> size;
  assert(!std::cin.bad());

  ull* array = new ull[size];
  for (size_t i = 0; i != size; ++i)
    std::cin >> array[i];

  LSDSort(array, size);

  for (size_t i = 0; i != size; ++i)
    std::cout << array[i] << ' ';

  delete[] array;
}
