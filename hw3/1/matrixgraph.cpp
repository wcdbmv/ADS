#include "matrixgraph.h"
#include <cassert>

MatrixGraph::MatrixGraph(int n) noexcept : adjacency_(n, std::vector<int>(n)) {}

MatrixGraph::MatrixGraph(const IGraph &other) noexcept
    : adjacency_(other.VerticesCount(), std::vector<int>(other.VerticesCount())) {
  for (int from = 0; from != adjacency_.size(); ++from)
    for (auto to: other.GetNextVertices(from))
      AddEdge(from, to);
}

bool MatrixGraph::VertexExists(int vertex) const {
  return 0 <= vertex && vertex < adjacency_.size();
}

void MatrixGraph::AddEdge(int from, int to) {
  assert(VertexExists(from));
  assert(VertexExists(to));
  adjacency_[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
  return adjacency_.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
  std::vector<int> result;

  for (int to = 0; to != adjacency_.size(); ++to)
    if (adjacency_[vertex][to] == 1)
      result.push_back(to);

  return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
  std::vector<int> result;

  for (int from = 0; from != adjacency_.size(); ++from)
    if (adjacency_[from][vertex])
      result.push_back(from);

  return result;
}

