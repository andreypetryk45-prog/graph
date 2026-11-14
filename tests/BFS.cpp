#include <iostream>
#include "../Pair.h"
#include "../List.h"
#include "../Vector.h"
#include "../Weighted_graph.h"

int main()
{
    Weighted_graph<int> graph;

    // Створюємо 7 вершин: 0, 1, 2, 3, 4, 5, 6
    for (int i = 0; i < 7; ++i)
        graph.add_vertex();

    // Додаємо орієнтовані ребра
    graph.add_edge(0, 1, 10);
    graph.add_edge(0, 2, 20);

    graph.add_edge(1, 3, 30);
    graph.add_edge(1, 4, 40);

    graph.add_edge(2, 4, 50);
    graph.add_edge(2, 5, 60);

    graph.add_edge(3, 6, 70);
    graph.add_edge(4, 6, 80);

    graph.add_vertex();
    // BFS починаючи з вершини 0
    Vector<int> result = graph.breadth_first_search(0);

    std::cout << "BFS traversal: ";

    for (int i = 0; i < result.size(); ++i)
        std::cout << result[i] << ' ';

    std::cout << '\n';
}