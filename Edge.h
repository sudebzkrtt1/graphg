#ifndef EDGE_H
#define EDGE_H

class Edge {
public:
    int from;
    int to;
    Edge* next;

    Edge(int from, int to);
};

#endif // EDGE_H
