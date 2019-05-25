#include <iostream>
#include "arcgraph.h"
#include "listgraph.h"
#include "matrixgraph.h"
#include "setgraph.h"

int main() {
  ListGraph list_graph(5);
  list_graph.AddEdge(0, 1);
  list_graph.AddEdge(0, 2);
  list_graph.AddEdge(1, 3);
  list_graph.AddEdge(2, 4);
  list_graph.AddEdge(3, 4);
  std::cout << list_graph << std::endl;

  ArcGraph arc_graph(list_graph);
  std::cout << arc_graph << std::endl;

  MatrixGraph matrix_graph(arc_graph);
  std::cout << matrix_graph << std::endl;

  SetGraph set_graph(matrix_graph);
  std::cout << set_graph << std::endl;

  return 0;
}
