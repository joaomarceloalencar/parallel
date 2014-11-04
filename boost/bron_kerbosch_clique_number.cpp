// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include "helper.hpp"
#include <fstream>

using namespace std;
using namespace boost;

// Declare the graph type and its vertex and edge types.
typedef undirected_graph<> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;


#define SIZE 40000

int main(int argc, char *argv[])
{
    // Create the graph and read it from standard input.
    Graph g;
    // read_graph(g, cin);
    
    boost::graph_traits<Graph>::vertex_descriptor v[SIZE];
   
    for (int i = 0; i < SIZE; i++)
       v[i] = g.add_vertex();   
  
    // boost::graph_traits<Graph>::vertex_descriptor v0 = g.add_vertex();
    // boost::graph_traits<Graph>::vertex_descriptor v1 = g.add_vertex();
    // boost::graph_traits<Graph>::vertex_descriptor v2 = g.add_vertex();


    // Ler arquivo 
    std::ifstream infile("path.txt-131072E-32768V-4NoZero-Random");
    int a, b, d;
    while (infile >> a >> b >> d)
    {
       // std::cout << a << "->" << b << std::endl;
       // Criar a aresta
       g.add_edge(v[a],v[b]);
    }  
 	
    // g.add_edge(v[1], v[2]);


    // Use the Bron-Kerbosch algorithm to find all cliques, and
    size_t c = bron_kerbosch_clique_number(g);
    cout << "clique number: " << c << endl;

    return 0;
}
