#include "Weighted_graph.h"
#include "Vector.h"
#include <iostream>
#include <fstream>   // to work with a file
#include <sstream>   // to get a stream from a string(line)
#include <string>    // to get a substring
#include <stdexcept> // to throw exceptions

Weighted_graph<int> read_graph(std::ifstream &file)
{

    if (!file.is_open())
    {
        throw std::runtime_error("File can't be opened.");
    }
    Weighted_graph<int> graph; // crate an empty weighted graph

    std::string line; // create a line to read each line of a file, where each line stands for an adjacency list of a vertex,
    // a vertex is represented as a number of lines

    Vector<std::string> lines; // creating a vector to store file lines, it needs because
    // my graph won't be working if there're no all vertices added

    while (std::getline(file, line)) // read file until it ends
    {
        lines.push_back(line); // if a file hasn't hit EOF, there still exists a vertex to be added, even if it has no neigbors(out_edges)
        graph.add_vertex();    // silmutaniously adding vertex
    }

    for (size_t i = 0; i < lines.size(); i++) // going through each adjacency lists and adding all out_edges:
    {
        if (lines[i].empty() == 0) // if there's no out_edges -- skipping the iteration:
        {
            continue;
        }

        // handling the current line, i.e. all adjacent vertices to the current vertex:
        std::istringstream adj_list(lines[i]);    // creating a stream
        std::string edge;                         // to store the current piece of  a line
        while (std::getline(adj_list, edge, ';')) // cutting the adj_list to pieces separeted by ';'
        {
            if (edge.empty()) // in case when there's ';' in the end of a line
            {
                continue; // exiting the current loop
            }

            // since find() returns size_t:
            unsigned int comma_position = static_cast<unsigned int>(edge.find(',')); // searching for the position of a comma

            std::string vertex_string = edge.substr(0, comma_position);  // read the first symbol
            std::string weight_string = edge.substr(comma_position + 1); // read edge from 'comma_position' and that's it, since a string ends

            int to = std::stoi(vertex_string) - 1; // convert string to int, substractin 1, since in the graph there's no vertex 0
            int weight = std::stoi(weight_string);

            int from = i; 

            // adding an edge to the adjacency list of a from vertex:
            graph.add_edge(from, to, weight);
        }
    }

    return graph;
}