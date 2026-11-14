
#include "../Stack.h"
#include <iostream>

int main()
{
    Stack<std::string> words;

    words.push("Hello");
    words.push("World");
    words.push("C++");

    std::cout << words << '\n';

    Stack<std::string> copy = words;
    words.pop();

    std::cout << "Original: " << words << '\n';
    std::cout << "Copy: " << copy << '\n';
}