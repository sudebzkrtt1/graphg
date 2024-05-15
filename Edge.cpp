#include "Edge.h"

Edge::Edge(int from, int to) {
    this->from = from;
    this->to = to;
    this->next = nullptr;
}
