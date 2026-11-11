#pragma once

#include <iostream>

template <typename T>
class Pair
{
private:
    int n_vertex;
    T m_weight;

public:
    Pair(int first_value, const T &second_value) : n_vertex(first_value), m_weight(second_value) {}
    int vertex() const
    {
        return n_vertex;
    }
    const T &weight() const // to not copy, a const reference is returned
    {
        return m_weight;
    }
    void set_weight(const T &weight) //to modify 'm_weight' outside the class
    {
        m_weight = weight;
    }
};