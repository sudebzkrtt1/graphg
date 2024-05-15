#ifndef EDGELIST_H
#define EDGELIST_H

#include "Edge.h"

class EdgeList {
public:
    Edge* head;
    Edge* tail;

    EdgeList();
    void insert(Edge* newEdge);
    bool contains(int to);
};

#endif // EDGELIST_H
