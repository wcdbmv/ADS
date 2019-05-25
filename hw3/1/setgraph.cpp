#include "setgraph.h"
#include <cassert>

SetGraph::SetGraph(int n) noexcept : vertices_(n) {}

SetGraph::SetGraph(const IGraph& other) noexcept : vertices_(other.VerticesCount()) {
  for (int from = 0; from != vertices_.size(); ++from) {
    for (auto to: other.GetNextVertices(from))
      AddEdge(from, to);
  }
}

bool SetGraph::VertexExists(int vertex) const {
  return 0 <= vertex && vertex < vertices_.size();
}

void SetGraph::AddEdge(int from, int to) {
  assert(VertexExists(from));
  assert(VertexExists(to));
  vertices_[from].insert(to);
}

int SetGraph::VerticesCount() const {
  return vertices_.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
  std::vector<int> result;

  for (auto i: vertices_[vertex])
    result.push_back(i);

  return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
  std::vector<int> result;

  for (int i = 1; i < vertices_.size(); ++i) {
    for (auto j: vertices_[i])
      if (j == vertex)
        result.push_back(i);
  }

  return result;
}
