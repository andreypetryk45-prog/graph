#pragma once

#include <iostream>
#include <stdexcept>
#include "Pair.h"
#include "List.h"
#include "Vector.h"

template <typename T> // this is the weight data type
class Weighted_graph
{
private:
    Vector<List<Pair<T>>> adjacency_vector;
    // graph is represented as a vector of adjacency lists
    // each index of the vector represents a vertex
    // each list stores vertices adjecent to that vertex
    // each list element is a Pair containing a vertex and an edge weight

public:
    Weighted_graph() = default; // constructing an empty vector using Vector default constructor: Vector()

    int size() const // returns the number of vertices
    {
        return this->adjacency_vector.size();
    }
    bool has_vertex(int vertex) const
    {
        return !(vertex < 0 || vertex >= size());
    }
    bool has_edge(int from, int to) const // checks if there's is an edge between them
    {
        if (!has_vertex(from) || !has_vertex(to))
        {
            throw std::out_of_range("There can't be such values as " +
                                    std::to_string(from) + ", " + std::to_string(to));
        }

        const List<Pair<T>> &temp = this->adjacency_vector[from]; // an adjacency list of 'from'
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
    int add_vertex() // add a vertex to a vector
    {
        adjacency_vector.push_back(List<Pair<T>>()); // tail = nullptr, head = nullptr, m_size = 0
        return adjacency_vector.size() - 1;
    }
    void erase_vertex(int ID) // erasing a vertex
    {
        if (!has_vertex(ID))
        { // in order to not erase a vertex that doesn't exist
            return;
        }

        Vector<List<Pair<T>>> &temporary = this->adjacency_vector; // creating a reference to work with a vector
        for (List<Pair<T>> &list : temporary)                      // removing all vetrices from other lists
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
            int list_size = temporary[i].get_size();
            typename List<Pair<T>>::Iterator it = temporary[i].begin();

        }
    }
    ~Weighted_graph() = default; // destroying a grapg using Vector's default destructor, which invokes ~List() then
};