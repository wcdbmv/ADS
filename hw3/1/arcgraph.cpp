#include "arcgraph.h"
#include <cassert>

ArcGraph::ArcGraph(int n) noexcept : n_vertices_(n) {}

ArcGraph::ArcGraph(const IGraph &other) noexcept : n_vertices_(other.VerticesCount()) {
  for (int from = 0; from < n_vertices_; ++from)
    for (int to : other.GetNextVertices(from))
      AddEdge(from, to);
}

bool ArcGraph::VertexExists(int vertex) const {
  return 0 <= vertex && vertex < n_vertices_;
}

void ArcGraph::AddEdge(int from, int to) {
  assert(VertexExists(from));
  assert(VertexExists(to));
  edges_.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
  return n_vertices_;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
  std::vector<int> result;

  for (const auto& edge: edges_)
    if (edge.first == vertex)
      result.push_back(edge.second);

  return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
  std::vector<int> result;

  for (const auto& edge: edges_)
    if (edge.second == vertex)
      result.push_back(edge.first);

  return result;
}

