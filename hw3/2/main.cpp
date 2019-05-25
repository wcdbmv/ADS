/*
 * Дан невзвешенный неориентированный граф. В графе может быть несколько
 * кратчайших путей между какими-то вершинами. Найдите количество различных
 * кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
 *
 * Формат ввода.
 * v: кол-во вершин (макс. 50000),
 * n: кол-во ребер (макс. 200000),
 * n пар реберных вершин,
 * пара вершин u, w для запроса.
 *
 * Формат вывода.
 * Количество кратчайших путей от u к w.
 */

#include <cassert>
#include <iostream>
#include <vector>
#include <queue>

class ListGraph final {
 public:

  explicit ListGraph(int n) noexcept;

  void AddEdge(int from, int to);
  int VerticesCount() const;

  const std::vector<int>& GetNextVertices(int vertex) const;

 private:
  std::vector<std::vector<int>> adjacency_;
};

int NumShortestWays(const ListGraph& graph, int from, int to) {
  // pair<n_shortest_ways, distance>
  std::vector<std::pair<int, int>> states(graph.VerticesCount());
  std::queue<int> queue;

  queue.push(from);
  states[from].first = 1;

  while (!queue.empty()){
    int vertex = queue.front();
    queue.pop();

    const int new_path = states[vertex].second + 1;
    for (auto i : graph.GetNextVertices(vertex)) {
      if (!states[i].first) {
        queue.push(i);
        states[i].first = states[vertex].first;
        states[i].second = new_path;
      }
      else if (new_path == states[i].second) {
        states[i].first += states[vertex].first;
      }
      else if (new_path < states[i].second) {
        states[i].first += states[vertex].first;
        states[i].second = new_path;
      }
    }
  }

  return states[to].first;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int v, e;
  std::cin >> v >> e;
  ListGraph graph(v);

  for (int i = 0; i < e; ++i) {
    int from, to;
    std::cin >> from >> to;
    graph.AddEdge(from, to);
  }

  int from, to;
  std::cin >> from >> to;

  std::cout << NumShortestWays(graph, from, to) << std::endl;

  return 0;
}

//——————————————————————————————————————————————————————————————————————————————

ListGraph::ListGraph(int n) noexcept: adjacency_(n) {}

void ListGraph::AddEdge(int from, int to) {
  adjacency_[from].push_back(to);
  adjacency_[to].push_back(from);
}

int ListGraph::VerticesCount() const {
  return adjacency_.size();
}

const std::vector<int>& ListGraph::GetNextVertices(int vertex) const {
  return adjacency_[vertex];
}
