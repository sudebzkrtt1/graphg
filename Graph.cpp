#include "Graph.h"

Graph::Graph(int vertexCount) {
    this->vertexCount = vertexCount;
    edges = new EdgeList[vertexCount];
}

Graph::~Graph() {
    delete[] edges;
}

void Graph::addEdge(int from, int to) {
    Edge* newEdge = new Edge(from, to);
    edges[from].insert(newEdge);
}
