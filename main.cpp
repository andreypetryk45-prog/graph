#include <iostream>
#include "fstream"
#include "Weighted_graph.h"
#include "read_graph_from_file.h"

int main()
{
    std::ifstream file("graph.txt");
    Weighted_graph<int> graph = read_graph(file);
    std::cout << graph;

    return 0;
}