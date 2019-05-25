#ifndef ARCGRAPH_H_
#define ARCGRAPH_H_

#include "igraph.h"

class ArcGraph : public IGraph {
 public:
  explicit ArcGraph(int n) noexcept;
  explicit ArcGraph(const IGraph& other) noexcept;

  void AddEdge(int from, int to) override;

  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 protected:
  std::vector<std::pair<int, int>> edges_;
  int n_vertices_;

 private:
  bool VertexExists(int vertex) const;
};

#endif  // ARCGRAPH_H_
