#ifndef IGRAPH_H_
#define IGRAPH_H_

#include <vector>
#include <ostream>

struct IGraph {
  virtual ~IGraph() {}

  virtual void AddEdge(int from, int to) = 0;

  virtual int VerticesCount() const  = 0;

  virtual std::vector<int> GetNextVertices(int vertex) const = 0;
  virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

std::ostream& operator<<(std::ostream& os, const IGraph& graph);

#endif  // IGRAPH_H_
