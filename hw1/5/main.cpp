/*
 * Группа людей называется современниками если был такой момент, когда они могли
 * собраться вместе. Для этого в этот момент каждому из них должно было уже
 * исполниться 18 лет, но ещё не исполниться 80 лет.
 * Дан список Жизни Великих Людей. Необходимо получить максимальное количество
 * современников. В день 18-летия человек уже может принимать участие в
 * собраниях, а в день 80-летия и в день смерти уже не может.
 * Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия.
 * В этих случаях принимать участие в собраниях он не мог.
 *
 * Использовать сортировку слиянием.
 */

// Moя идея решения состоит в том, чтобы усечь интервалы времени до таких, в
// которые Великие люди могли собираться (с 18 до 80 лет), далее остортировать
// даты, запомнив, какие из них начальные, какие — конечные, затем, условно
// представив начальные даты открывающимися скобками, а конечные —
// закрывающимися, найти наибольшую глубину скобочной последовательности.

#include <algorithm>
#include <cassert>
#include <iostream>

static const int ADULT_AGE = 18;
static const int ELDERLY_AGE = 80;

struct BinaryDate {
  explicit BinaryDate();
  explicit BinaryDate(int day, int month, int year, bool bit);
  ~BinaryDate() {}

  int day;
  int month;
  int year;
  bool bit;  // is opening bracket ?
};

BinaryDate::BinaryDate() : day(0), month(0), year(0), bit(0) {}

BinaryDate::BinaryDate(int day, int month, int year, bool bit)
    : day(day), month(month), year(year), bit(bit) {}

bool operator<(const BinaryDate& a, const BinaryDate& b) {
  if (a.year < b.year)
    return true;
  if (a.year > b.year)
    return false;
  if (a.month < b.month)
    return true;
  if (a.month > b.month)
    return false;
  return a.day < b.day;
}

bool operator==(const BinaryDate& a, const BinaryDate& b) {
  return a.year == b.year
      && a.month == b.month
      && a.day == b.day;
}

bool operator<=(const BinaryDate& a, const BinaryDate& b) {
  return a < b || (a == b && !a.bit);
}

std::istream& operator>>(std::istream& is, BinaryDate& binary_date) {
  is >> binary_date.day
     >> binary_date.month
     >> binary_date.year;
  binary_date.bit = 0;
  return is;
}

struct GreatPerson {
  explicit GreatPerson();
  explicit GreatPerson(int bd, int bm, int by, int dd, int dm, int dy);

  bool adult() const { return pass(ADULT_AGE); }
  bool elderly() const { return pass(ELDERLY_AGE); }

  void shift();

  const BinaryDate& get_birth() const { return birth; }
  const BinaryDate& get_death() const { return death; }

  std::istream& operator>>(std::istream& is);

  BinaryDate birth, death;
  bool shifted;

  bool pass(int year) const;
};

GreatPerson::GreatPerson()
    : birth(BinaryDate()), death(BinaryDate()), shifted(false) {}

GreatPerson::GreatPerson(int bd, int bm, int by, int dd, int dm, int dy)
    : birth(BinaryDate(bd, bm, by, 1)),
      death(BinaryDate(dd, dm, dy, 0)),
      shifted(false) {}

bool GreatPerson::pass(int year) const {
  assert(!shifted);
  BinaryDate age = birth;
  age.year += year;
  return age < death;
}

void GreatPerson::shift() {
  assert(!shifted && adult());
  if (elderly()) {
    birth.year += ADULT_AGE;
    death = birth;
    death.year += ELDERLY_AGE - ADULT_AGE;
    death.bit = false;
  } else {
    // Замечу, что повторяющийся блок не может быть вынесен отдельно, так как
    // это повлияет на результат функции elderly()
    birth.year += ADULT_AGE;
  }
  shifted = true;
}

std::istream& operator>>(std::istream& is, GreatPerson& great_person) {
  is >> great_person.birth >> great_person.death;
  great_person.birth.bit = true;
  return is;
}

template <typename T>
void Merge(T* a, size_t li, size_t mi, size_t ri, T* b) {
  size_t i = li, j = mi;
  for (size_t k = li; k != ri; ++k)
    if (i < mi && (j >= ri || a[i] <= a[j]))
      b[k] = a[i++];
    else
      b[k] = a[j++];
}

template <typename T>
void SplitMerge(T* b, size_t li, size_t ri, T* a) {
  if (ri - li < 2)
    return;

  size_t mi = li + (ri - li) / 2;
  SplitMerge<T>(a, li, mi, b);
  SplitMerge<T>(a, mi, ri, b);

  Merge<T>(b, li, mi, ri, a);
}

template <typename T>
void CopyArray(T* a, T* b, size_t n) {
  for (size_t i = 0; i != n; ++i)
    a[i] = b[i];
}

template <typename T>
void MergeSort(T* a, size_t n) {
  T* b = new T[n];
  CopyArray<T>(b, a, n);
  SplitMerge<T>(b, 0, n, a);
  delete[] b;
}

int ReadNum() {
  int num;
  std::cin >> num;
  assert(!std::cin.bad());
  return num;
}

GreatPerson* ReadGreatPersons(size_t size) {
  assert(size);
  GreatPerson* great_persons = new GreatPerson[size];
  for (size_t i = 0; i != size; ++i)
    std::cin >> great_persons[i];
  return great_persons;
}

BinaryDate* ExtractBinaryDates(GreatPerson* persons, size_t n, size_t *m) {
  assert(persons && n && m);
  BinaryDate* dates = new BinaryDate[2 * n];
  *m = 0;
  for (size_t i = 0; i != n; ++i)
    if (persons[i].adult()) {
      persons[i].shift();
      dates[(*m)++] = persons[i].get_birth();
      dates[(*m)++] = persons[i].get_death();
    }
  if (*m) {
    if (*m != 2 * n) {
      BinaryDate* tmp = new BinaryDate[*m];
      CopyArray<BinaryDate>(tmp, dates, *m);
      delete[] dates;
      dates = tmp;
    }
  } else {
    delete[] dates;
    dates = NULL;
  }

  return dates;
}

size_t MaxDepth(BinaryDate* dates, size_t n) {
  size_t max_depth = 0;
  size_t cur_depth = 0;
  for (size_t i = 0; i != n; ++i) {
    cur_depth += 2 * dates[i].bit - 1;
    max_depth = std::max(cur_depth, max_depth);
  }
  return max_depth;
}

inline void DeleteGreatPersons(GreatPerson* great_persons) {
  delete[] great_persons;
}

inline void DeleteBinaryDates(BinaryDate* binary_dates) {
  delete[] binary_dates;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int n = ReadNum();
  assert(n > 0);

  GreatPerson* great_persons = ReadGreatPersons(n);

  size_t m = 0;
  BinaryDate* dates = ExtractBinaryDates(great_persons, n, &m);
  if (!dates) {
    std::cout << 0;
    return 0;
  }

  MergeSort<BinaryDate>(dates, m);

  std::cout << MaxDepth(dates, m);

  DeleteGreatPersons(great_persons);
  DeleteBinaryDates(dates);
}
