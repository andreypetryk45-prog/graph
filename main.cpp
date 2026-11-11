#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <cstdlib>
#include <new>
#include "Vector.h"

int main()
{
    Vector<int> vector{};
    for (int i = 0; i < 100000; i++)
    {
        vector.push_back(i);
    }
    int counter = 0;
    const Vector<int> const_vector = vector; // deep-copy

    bool is_equal = vector == const_vector;
    std::cout << std::boolalpha << is_equal;

    return 0;
}