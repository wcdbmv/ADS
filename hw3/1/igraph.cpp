#include "igraph.h"

std::ostream& operator<<(std::ostream& os, const IGraph& graph) {
  os << '[';
  int count = 0;
  for (int from = 0; from != graph.VerticesCount(); ++from)
    for (int to: graph.GetNextVertices(from)) {
      if (count)
        os << ", ";
      os << from << " -> " << to;
      ++count;
    }
  os << ']';
  return os;
}
