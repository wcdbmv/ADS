#ifndef MATRIXGRAPH_H_
#define MATRIXGRAPH_H_

#include "igraph.h"

class MatrixGraph : public IGraph {
 public:
  explicit MatrixGraph(int n) noexcept;
  explicit MatrixGraph(const IGraph& other) noexcept;

  void AddEdge(int from, int to) override;

  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 protected:
  std::vector<std::vector<int>> adjacency_;

 private:
  bool VertexExists(int vertex) const;
};
#endif  // MATRIXGRAPH_H_
