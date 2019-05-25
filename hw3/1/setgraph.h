#ifndef SETGRAPH_H_
#define SETGRAPH_H_

#include <unordered_set>
#include "igraph.h"

class SetGraph : public IGraph {
 public:
  SetGraph(int n) noexcept;
  SetGraph(const IGraph& other) noexcept;

  void AddEdge(int from, int to) override;

  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 protected:
  std::vector<std::unordered_set<int>> vertices_;

 private:
  bool VertexExists(int vertex) const;
};

#endif  // SETGRAPH_H_
