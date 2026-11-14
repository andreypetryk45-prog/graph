#pragma once

#include <iostream>
#include <stdexcept>
#include "Pair.h"
#include "List.h"
#include "Vector.h"
#include "Queue.h"
#include "Stack.h"

template <typename T> // this is the weight data type
class Weighted_graph
{
private:
    Vector<List<Pair<T>>> adj_list;
    // graph is represented as a vector of adjacency lists
    // each index of the vector represents a vertex
    // each list stores vertices adjecent to that vertex
    // each list element is a Pair containing a vertex and an edge weight

    void recursion(int start, Vector<int> &result, Vector<bool> &visited) const
    {
        visited[start] = true;
        result.push_back(start);
        for (auto &node : adj_list[start])
        {
            if (!visited[node.vertex()])
            {
                recursion(node.vertex(), result, visited);
            }
        }
        // result.push_back(start); if post_order is neeeded
    }

public:
    Weighted_graph() = default; // constructing an empty vector using Vector default constructor: Vector()

    Vector<bool> incident_list() const // cheсks if there's an isolated vertex
    {
        Vector<bool> incident_list(adj_list.size()); // creating a vector telling if there's an edge
        for (auto &element : incident_list)
        {
            element = false;
        }

        for (int i = 0; i < adj_list.size(); i++) // moving along all verices
        {
            for (const auto &node : adj_list[i]) // moving along list's nodes
            {
                incident_list[i] = true; // i-th vertix has one out_edge
                // but this also means that node.vertex() has out_edge that is in_edge for i
                incident_list[node.vertex()] = true; // i-th has one out_edge
            }
        }
        return incident_list;
    }
    bool has_isolated_vertex() const
    {
        const Vector<bool> temporary = incident_list();
        for (const bool value : temporary)
        {
            if (value == false)
            {
                return true;
            }
        }
        return false;
    }
    bool is_vertex(int vertex) const
    {
        return !(vertex < 0 || vertex >= size());
    }
    bool has_edge(int from, int to) const // checks if there's is an edge between them
    {
        if (!is_vertex(from) || !is_vertex(to))
        {
            throw std::out_of_range("There can't be such values as " +
                                    std::to_string(from) + ", " + std::to_string(to));
        }

        const List<Pair<T>> &temp = this->adj_list[from]; // an adjacency list of 'from'
        // no copy is created/ Instead, we crate a const refernece to the existing object:
        if (temp.is_empty())
        {
            return false; // if an adgacency list is empty, there can't be an adge with 'to' in it
        }

        for (const Pair<T> &node : temp)
        { // moving along all nodes of a list
            if (node.vertex() == to)
            { // if there's at least one vertex equal to 'to', then there's an adge between them
                return true;
            }
        }
        return false;
    }

    int size() const // returns the number of vertices
    {
        return this->adj_list.size();
    }
    int add_vertex() // add a vertex to a vector
    {
        adj_list.push_back(List<Pair<T>>()); // tail = nullptr, head = nullptr, m_size = 0
        return adj_list.size() - 1;
    }
    int out_degree(int vertex) const // returns the number of out_edges
    {
        if (!is_vertex(vertex))
        {
            throw std::out_of_range("There can't bu such a value as " + std::to_string(vertex));
        }
        return adj_list[vertex].size();
    }
    int in_degree(int vertex) const // returns the number of in_edges
    {
        if (!is_vertex(vertex))
        {
            throw std::out_of_range("There can't bu such a value as " + std::to_string(vertex));
        }
        const auto &temp = adj_list;
        int counter = 0;
        for (int i = 0; i < temp.size(); i++)
        {
            for (const auto &node : temp[i])
            {
                if (node.vertex() == vertex)
                {
                    counter++;
                }
            }
        }
        return counter;
    }
    void add_edge(int from, int to, const T &weight)
    {
        if (!is_vertex(from) || !is_vertex(to))
        {
            throw std::out_of_range("There can't bu such a value as " + std::to_string(from) +
                                    ", " + std::to_string(to));
        }
        if (from == to)
        {
            throw std::invalid_argument("There can't exist an loop in this graph!");
        }
        if (has_edge(from, to))
        {
            throw std::invalid_argument("There alredy exists an edge betwen these vertices!");
        }
        adj_list[from].push_back(Pair<T>(to, weight));
    }
    void erase_vertex(int ID) // erasing a vertex
    {
        if (!is_vertex(ID))
        { // in order to not erase a vertex that doesn't exist
            throw std::out_of_range("There can't be such values as " +
                                    std::to_string(ID));
        }

        Vector<List<Pair<T>>> &temporary = this->adj_list; // creating a reference to work with a vector
        for (List<Pair<T>> &list : temporary)              // removing all vetrices from other lists
        {
            list.remove_if(
                [&ID](const Pair<T> &element)
                {
                    return ID == element.vertex();
                });
        }

        temporary.erase(ID); // erasing the vertex

        // Since all vertices' IDs have been shifted to the left, starting from index == ID,
        // we need to decrease them by 1

        for (int i = 0; i < temporary.size(); i++)
        {
            for (auto &node : temporary[i])
            {
                if (node.vertex() > ID)
                {
                    int new_vertex = node.vertex() - 1;
                    node.set_vertex(new_vertex);
                }
            }
        }
    }
    void erase_edge(int from, int to) // deletes the edge between 'from' and 'to'
    {
        if (!(is_vertex(from)) || !(is_vertex(to)))
        {
            throw std::out_of_range("There can't be such values as " +
                                    std::to_string(from) + ", " + std::to_string(to));
        }
        List<Pair<T>> &list = this->adj_list[from];
        list.remove_if([&to](const Pair<T> &element)
                       { return to == element.vertex(); });
    }
    const List<Pair<T>> &neighbors(int vertex) const
    {
        if (!is_vertex(vertex))
        {
            throw std::out_of_range("Vertex does not exist!");
        }
        return adj_list[vertex];
    }
    // search and backtracking methods:
    Vector<int> breadth_first_search(int start) const
    {
        if (!is_vertex(start))
        {
            throw std::out_of_range("There can't bu such a value as " + std::to_string(start));
        }
        Vector<int> result{};                  // used to store the vertices in the order BFS moves along
        Vector<bool> visited(adj_list.size()); // used to save visited vertices, the vector is of the size equal to the number of vertices in the graph
        for (int i = 0; i < adj_list.size(); i++)
        {
            visited[i] = false;
        }
        Queue<int> queue{};

        visited[start] = true; // mark this vertex as visited
        queue.enqueue(start);  // adds start in queue

        while (!queue.is_empty()) // iterating 'till queue handls all vertices
        {
            const int current_front = queue.front();
            queue.dequeue(); // deleting the vertex that has been added the very first

            result.push_back(current_front); // since we've deleted it fron the queue, it's added to the result vector
            // Now we're handling its neigbours:
            for (auto &node : adj_list[current_front])
            {
                int neighbor = node.vertex();
                // checking if it hasn't already been visited:
                if (!visited[neighbor])
                {
                    queue.enqueue(neighbor);  // adding it to the queue, since it hasn't been proceeded yet
                    visited[neighbor] = true; // Since we've already visited it, mark it as visited
                }
            }
        }
        return result;
    }
    Vector<int> shortest_path(int start, int destination) const
    {
        if (!is_vertex(start) || !is_vertex(destination))
        {
            throw std::out_of_range("There can't bu such a value as " +
                                    std::to_string(start) + std::to_string(destination));
        }
        if (start == destination)
        {
            Vector<int> temporary{};
            temporary.push_back(destination);
            return temporary;
        }
        Vector<int> parent(adj_list.size()); // used for storing the parent of a vertex: the index is a vertex and what's stored in it -- its parent

        for (unsigned int i = 0; i < adj_list.size(); i++)
        {
            parent[i] = -1;
        }

        Vector<bool> visited(adj_list.size()); // used to save visited vertices, the vector is of the size equal to the number of vertices in the graph

        for (int i = 0; i < adj_list.size(); i++)
        {
            visited[i] = false;
        }

        Queue<int> queue{};
        visited[start] = true; // mark this vertex as visited
        queue.enqueue(start);  // adds start in queue

        while (!queue.is_empty()) // iterating 'till queue handls all vertices
        {
            const int current_front = queue.front();
            queue.dequeue(); // deleting the vertex that has been added the very first

            // Now we're handling its neigbours:
            for (auto &node : adj_list[current_front])
            {
                int neighbor = node.vertex();
                // checking if it hasn't already been visited:
                if (!visited[neighbor])
                {
                    parent[neighbor] = current_front;
                    queue.enqueue(neighbor);  // adding it to the queue, since it hasn't been proceeded yet
                    visited[neighbor] = true; // Since we've already visited it, mark it as visited
                }
            }
        }
        if (!visited[destination])
        {
            return Vector<int>{}; // it's an isolated vertex or it's not bound with start
        }
        Vector<int> path{};
        int current = destination;
        while (current != start)
        {
            path.push_back(current);
            current = parent[current];
        }
        path.push_back(start);
        Vector<int> temporary{};
        for (unsigned int i = path.size(); i > 0; i--)
        {
            temporary.push_back(path[i - 1]);
        }
        return temporary;
    }
    Vector<int> depth_first_search(int start) const
    {
        if (!is_vertex(start))
        {
            throw std::out_of_range("There can't bu such a value as " + std::to_string(start));
        }
        Vector<bool> visited(adj_list.size());
        for (int i = 0; i < adj_list.size(); i++)
        {
            visited[i] = false;
        }
        Vector<int> result;

        for (const auto &node : adj_list[start])
        {
            if (!visited[node.vertex()])
            {
                recursion(start, result, visited);
            }
        }

        return result;
    }
    Vector<int> depth_first_search_iterative(int start) const
    {
        if (!is_vertex(start))
        {
            throw std::out_of_range("There can't bu such a value as " + std::to_string(start));
        }

        Stack<int> vertices{};
        Stack<int> neighbor_indices{}; // used to store the next neighbor
        Vector<int> result{};
        Vector<bool> visited(adj_list.size());
        for (int i = 0; i < adj_list.size(); i++)
        {
            visited[i] = false;
        }

        vertices.push(start); // it should leave Stack the last
        neighbor_indices.push(0);
        visited[start] = true; // mark start as visited
        result.push_back(start);

        while (!vertices.is_empty())
        {
            unsigned int current = vertices.top(); // getting the current top of Stack

            if (neighbor_indices.top() == adj_list[current].size())
            {
                vertices.pop();
                neighbor_indices.pop();
                continue;
            }
            else
            {
                unsigned int index = neighbor_indices.top();
                unsigned int neighbor = adj_list[current][index].vertex();
                neighbor_indices.top()++;
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    vertices.push(neighbor);
                    neighbor_indices.push(0);
                    result.push_back(neighbor);
                }
            }
        }
        return result;
    }

    Vector<int> dijkstra(Weighted_graph<double> &graph, int from, int to)
    {
        if (graph.size() == 0)
            throw std::out_of_range("Graph is empty!");
        if (!graph.is_vertex(from))
            throw std::invalid_argument("There can't be such vertices as: " +
                                        std::to_string(from));
        if (!graph.is_vertex(to))
            throw std::invalid_argument("There can't be such vertices as: " +
                                        std::to_string(to));

        const double infinity = std::numeric_limits<double>::infinity();

        // use this for saving the shortest path to each vertex
        Vector<double> distances(graph.size());
        for (int i = 0; i < graph.size(); i++)
        {
            distances[i] = infinity; // fill the vector of distances as such that the distance to each vertex is infinite
        }
        distances[from] = 0; // mark the distanse from 'from' as 0 before the iteration start

        Vector<int> previous(graph.size());
        for (int i = 0; i < graph.size(); i++)
        {
            previous[i] = -1; // as of now, mark all vertices as those that don't have the previous vertex
        }
        // the 'from' vertex won't have the previous node

        Vector<bool> visited(graph.size());
        for (int i = 0; i < graph.size(); i++)
        {
            visited[i] = false; // ,ark each veertex as such that hasn't been visitted yet
        }

        while (true)
        {
            int curr_min = -1;
            double min_distance = infinity;

            for (int i = 0; i < graph.size(); i++)
            {
                if (!visited[i] && distances[i] < min_distance)
                {

                    min_distance = distances[i];
                    curr_min = i;
                }
            }

            if (curr_min == -1) // there's no edge between 'from' and 'to'
            {
                break;
            }

            visited[curr_min] = true;
            if (curr_min == to)
            {
                break;
            }
            for (const auto &neighbor : graph.neighbors(curr_min))
            {
                if (!visited[neighbor.vertex()] && graph.has_edge(curr_min, neighbor.vertex()))
                {
                    double weight = neighbor.weight();
                    double new_dist = distances[curr_min] + weight;

                    if (new_dist < distances[neighbor.vertex()])
                    {
                        distances[neighbor.vertex()] = new_dist;
                        previous[neighbor.vertex()] = curr_min;
                    }
                }
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Weighted_graph &graph)
    {
        if (graph.size() == 0)
        {
            os << "[]";
            return os;
        }
        os << "\nGraph: \n";
        for (size_t i = 0; i < graph.size(); i++)
        {
            os << "Vertex " << i + 1 << ": "; // additon of 1, specifically for the graph in "graph.txt"
            if (graph.adj_list[i].is_empty()) // if a vertex has no out_edges:
            {
                os << "\n";
            }
            else
            {
                size_t counter = 0;
                for (const auto &node : graph.adj_list[i]) // going through each adjacency list:
                {
                    os << node.vertex()  + 1 << ", " << node.weight(); // the same applies here
                    counter++;
                    if (counter < graph.adj_list[i].size())
                    {
                        os << "; ";
                    }
                }
            }
            os << "\n";
        }
        return os;
    }
    ~Weighted_graph() = default; // destroying a grapg using Vector's default destructor, which invokes ~List() then
};