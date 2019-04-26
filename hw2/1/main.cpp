/*
 * Задача 1. Хеш-таблица
 *
 * Реализуйте структуру данных типа "множество строк" на основе динамической
 * хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
 * строчных латинских букв.
 *
 * Хеш-функция строки должна быть реализована с помощью вычисления значения
 * многочлена методом Горнера. Начальный размер таблицы должен быть равным
 * 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда
 * коэффициент заполнения таблицы достигает 3/4.
 *
 * Структура данных должна поддерживать операции добавления строки в множество,
 * удаления строки из множества и проверки принадлежности данной строки
 * множеству.
 *
 * Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
 * g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
 *
 * Формат входных данных
 * Каждая строка входных данных задает одну операцию над множеством. Запись
 * операции состоит из типа операции и следующей за ним через пробел строки,
 * над которой проводится операция.
 * Тип операции  – один из трех символов:
 *   +  означает добавление данной строки в множество;
 *   -  означает удаление  строки из множества;
 *   ?  означает проверку принадлежности данной строки множеству.
 * При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в
 * этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он
 * присутствует в этом множестве.
 *
 * Формат выходных данных
 * Программа должна вывести для каждой операции одну из двух строк OK или FAIL,
 * в зависимости от того, встречается ли данное слово в нашем множестве.
 */

#include <iostream>
#include <cassert>
#include <string>
#include <vector>

template <typename T, size_t THash(const T&, size_t)>
class HashTable {
 public:
  explicit HashTable(size_t size = kDefaultTableSize);
  ~HashTable();

  bool Insert(const T& key);
  bool Remove(const T& key);
  bool Has(const T& key) const;

 private:
  class Node {
   public:
    explicit Node(const T& key);

    T key;
    bool deleted;
  };

  std::vector<Node*> table_;
  size_t size_;


  void Rehash();

  size_t Hash(const T& key) const;
  size_t QuadraticProbe(size_t h, size_t i) const;
  size_t QuadraticProbe(size_t h, size_t i, size_t m) const;

  float fill_factor() const;

  constexpr static const size_t kDefaultTableSize = 8;
  constexpr static const float kDefaultRehashRatio = 0.75f;
  constexpr static const size_t kDefaultResizeFactor = 2;
  constexpr static const float kC1 = 0.5f;
  constexpr static const float kC2 = 0.5f;
};

size_t HashHorner(const std::string& key, size_t m);

void PrintResult(bool ok) { std::cout << (ok ? "OK" : "FAIL") << std::endl; }

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  HashTable<std::string, HashHorner> table;
  char command;
  std::string key;
  while (std::cin >> command >> key) {
    switch (command) {
      case '?':
        PrintResult(table.Has(key));
        break;
      case '+':
        PrintResult(table.Insert(key));
        break;
      case '-':
        PrintResult(table.Remove(key));
        break;
    }
  }
}

//——————————————————————————————————————————————————————————————————————————————

template <typename T, size_t THash(const T&, size_t)>
HashTable<T, THash>::Node::Node(const T& key)
    : key(key), deleted(false) {}

template <typename T, size_t THash(const T&, size_t)>
HashTable<T, THash>::HashTable(size_t size)
    : table_(size, nullptr), size_(0) {
  assert(size);
}

template <typename T, size_t THash(const T&, size_t)>
HashTable<T, THash>::~HashTable() {
  for (size_t i = 0; i != table_.size(); ++i)
    delete table_[i];
}

template <typename T, size_t THash(const T&, size_t)>
float HashTable<T, THash>::fill_factor() const {
  return static_cast<float>(size_) / table_.size();
}

template <typename T, size_t THash(const T&, size_t)>
bool HashTable<T, THash>::Insert(const T& key) {
  if (fill_factor() >= kDefaultRehashRatio)
    Rehash();

  const size_t hash = Hash(key);
  for (size_t i = 0; i != table_.size(); ++i) {
    const size_t probe = QuadraticProbe(hash, i);
    if (!table_[probe]) {
      table_[probe] = new Node(key);
      ++size_;
      return true;
    } else if (table_[probe]->deleted) {
      table_[probe]->key = key;
      table_[probe]->deleted = false;
      ++size_;
      return true;
    } else if (table_[probe]->key == key)
      return false;
  }

  return false;
}

template <typename T, size_t THash(const T&, size_t)>
bool HashTable<T, THash>::Remove(const T& key) {
  const size_t hash = Hash(key);
  for (size_t i = 0; i != table_.size(); ++i) {
    const size_t probe = QuadraticProbe(hash, i);
    if (!table_[probe]) {
      return false;
    } else if (!table_[probe]->deleted && table_[probe]->key == key) {
      table_[probe]->deleted = true;
      --size_;
      return true;
    }
  }

  return false;
}

template <typename T, size_t THash(const T&, size_t)>
bool HashTable<T, THash>::Has(const T& key) const {
  const size_t hash = Hash(key);
  for (size_t i = 0; i != table_.size(); ++i) {
    const size_t probe = QuadraticProbe(hash, i);
    if (!table_[probe])
      return false;
    if (!table_[probe]->deleted && table_[probe]->key == key)
      return true;
  }

  return false;
}

template <typename T, size_t THash(const T&, size_t)>
void HashTable<T, THash>::Rehash() {
  std::vector<Node*> old_table = table_;
  table_.assign(table_.size() * kDefaultResizeFactor, nullptr);
  for (size_t i = 0; i != old_table.size(); ++i)
    if (old_table[i]) {
      if (!old_table[i]->deleted)
        Insert(old_table[i]->key);
      delete old_table[i];
    }
}

template <typename T, size_t THash(const T&, size_t)>
size_t HashTable<T, THash>::Hash(const T& key) const {
  return THash(key, table_.size());
}

template <typename T, size_t THash(const T&, size_t)>
size_t HashTable<T, THash>::QuadraticProbe(size_t h, size_t i) const {
  return QuadraticProbe(h, i, table_.size());
}

template <typename T, size_t THash(const T&, size_t)>
size_t HashTable<T, THash>::QuadraticProbe(size_t h, size_t i, size_t m) const {
  return (h + (i + i * i) / 2) % m;
}

size_t HashHorner(const std::string& key, size_t m) {
  size_t hash = 0;
  for (size_t i = 0; i != key.size(); ++i)
    hash = (hash * 17 + key[i]) % m;
  return hash;
}
