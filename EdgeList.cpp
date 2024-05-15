#include "EdgeList.h"

EdgeList::EdgeList() {
    head = nullptr;
    tail = nullptr;
}

void EdgeList::insert(Edge* newEdge) {
    if (head == nullptr) {
        head = newEdge;
    } else {
        tail->next = newEdge;
    }
    tail = newEdge;
}

bool EdgeList::contains(int to) {
    Edge* current = head;
    while (current != nullptr) {
        if (current->to == to) {
            return true;
        }
        current = current->next;
    }
    return false;
}
