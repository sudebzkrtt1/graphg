#ifndef GRAPH_H
#define GRAPH_H

#include "EdgeList.h"

class Graph {
public:
    EdgeList* edges;
    int vertexCount;

    Graph(int vertexCount);
    ~Graph();
    void addEdge(int from, int to); // Add edge method declaration
};

#endif // GRAPH_H
