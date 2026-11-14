
#include <iostream>
#include <vector>
#include <utility>
#include <stdexcept>

class Graph
{
private:
    std::vector<std::vector<int>> adj;

public:
    explicit Graph(int vertices) : adj(vertices) {}

    void add_edge(int u, int v)
    {
        if (u < 0 || v < 0 ||
            u >= static_cast<int>(adj.size()) ||
            v >= static_cast<int>(adj.size()))
        {
            throw std::out_of_range("Invalid vertex");
        }

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> depth_first_search_iterative(int start) const
    {
        if (start < 0 || start >= static_cast<int>(adj.size()))
            throw std::out_of_range("Invalid start vertex");

        std::vector<int> vertices;
        std::vector<int> neighbor_indices;
        std::vector<int> result;
        std::vector<bool> visited(adj.size(), false);

        vertices.push_back(start);
        neighbor_indices.push_back(0);
        visited[start] = true;
        result.push_back(start);

        while (!vertices.empty())
        {
            int current = vertices.back();

            if (neighbor_indices.back() == static_cast<int>(adj[current].size()))
            {
                vertices.pop_back();
                neighbor_indices.pop_back();
                continue;
            }

            int index = neighbor_indices.back();
            int neighbor = adj[current][index];

            neighbor_indices.back()++;

            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                vertices.push_back(neighbor);
                neighbor_indices.push_back(0);
                result.push_back(neighbor);
            }
        }

        return result;
    }
};

int main()
{
    constexpr int VERTICES = 25;
    constexpr int START = 0;

    Graph graph(VERTICES);

    const std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 4}, {0, 7},
        {1, 2}, {1, 5},
        {2, 3}, {2, 8},
        {3, 4}, {3, 9},
        {4, 10},
        {5, 6}, {5, 11},
        {6, 7}, {6, 12},
        {7, 13},
        {8, 9}, {8, 14},
        {9, 15},
        {10, 11}, {10, 16},
        {11, 12}, {11, 17},
        {12, 13}, {12, 18},
        {13, 19},
        {14, 15}, {14, 20},
        {15, 16}, {15, 21},
        {16, 17}, {16, 22},
        {17, 18}, {17, 23},
        {18, 19}, {18, 24},
        {19, 20},
        {20, 21},
        {21, 22},
        {22, 23},
        {23, 24},
        {24, 0}
    };

    for (const auto& [u, v] : edges)
        graph.add_edge(u, v);

    std::cout << "Graph vertices: " << VERTICES << '\n';
    std::cout << "Graph edges: " << edges.size() << "\n\n";

    std::cout << "Starting DFS from vertex " << START << "...\n";

    std::vector<int> result =
        graph.depth_first_search_iterative(START);

    std::cout << "DFS completed.\n";
    std::cout << "Visited vertices: " << result.size() << "\n";

    std::cout << "Traversal order:\n";

    for (int vertex : result)
        std::cout << vertex << ' ';

    std::cout << '\n';

    return 0;
}
