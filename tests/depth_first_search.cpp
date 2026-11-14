
#include "../Pair.h"
#include "../List.h"
#include "../Vector.h"
#include "../Queue.h"
#include "../Weighted_graph.h"

#include <iostream>

int main()
{
    Weighted_graph<int> graph{};
    constexpr int V = 30;
    // Create 1000 vertices
    for (int i = 0; i < V; ++i)
        graph.add_vertex();

    // Create a connected directed graph
    for (int i = 0; i < V; ++i)
    {
        // Forward edges
        if (i + 1 < V)
            graph.add_edge(i, i + 1, 1 + (i * 7) % 20);

        if (i + 2 < V)
            graph.add_edge(i, i + 2, 1 + (i * 11) % 20);

        if (i + 5 < V)
            graph.add_edge(i, i + 5, 1 + (i * 13) % 20);

        // Longer jumps
        if (i + 10 < V)
            graph.add_edge(i, i + 10, 1 + (i * 17) % 20);

        if (i + 25 < V)
            graph.add_edge(i, i + 25, 1 + (i * 19) % 20);

        // Additional edges creating alternative routes
        if (i % 3 == 0 && i + 15 < V)
            graph.add_edge(i, i + 15, 1 + (i * 23) % 20);

        if (i % 7 == 0 && i + 30 < V)
            graph.add_edge(i, i + 30, 1 + (i * 29) % 20);
    }

    Vector<int> result = graph.depth_first_search(2);
    for (int i = 0; i < result.size(); i++)
    {
        std::cout << " -> " << result[i];
    }
    std::cout << std::endl
              << std::endl;
    graph.add_edge(5, 2, 0);
    result = graph.depth_first_search(2);
    for (int i = 0; i < result.size(); i++)
    {
        std::cout << " -> " << result[i];
    }
}