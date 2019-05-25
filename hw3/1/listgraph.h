#ifndef LISTGRAPH_H_
#define LISTGRAPH_H_

#include "igraph.h"

class ListGraph : public IGraph {
 public:
  explicit ListGraph(int n) noexcept;
  ListGraph(const IGraph& other) noexcept;

  void AddEdge(int from, int to) override;
  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 protected:
  std::vector<std::vector<int>> adjacency_;

 private:
  bool VertexExists(int vertex) const;
};

#endif  // LISTGRAPH_H_
