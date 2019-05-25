#include "listgraph.h"
#include <cassert>

ListGraph::ListGraph(int n) noexcept: adjacency_(n) {}

ListGraph::ListGraph(const IGraph& other) noexcept : adjacency_(other.VerticesCount()) {
  for (size_t i = 0; i < adjacency_.size(); ++i)
    adjacency_[i] = other.GetNextVertices(i);
}

bool ListGraph::VertexExists(int vertex) const {
  return 0 <= vertex && vertex < adjacency_.size();
}

void ListGraph::AddEdge(int from, int to) {
  assert(VertexExists(from));
  assert(VertexExists(to));
  adjacency_[from].push_back(to);
}

int ListGraph::VerticesCount() const {
  return adjacency_.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
  assert(VertexExists(vertex));
  return adjacency_[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
  assert(VertexExists(vertex));
  std::vector<int> prev_vertices;
  for (int from = 0; from < adjacency_.size(); ++from)
    for (int to: adjacency_[from])
      if (to == vertex)
        prev_vertices.push_back(from);
  return prev_vertices;
}




