#ifndef QTYPE_H
#define QTYPE_H

typedef int Key;

struct Item {
    Key key;
    int value;
};

struct Reply {
    bool success;
    Item item;
};

struct Node {
    Item item;
    Node* next;
};

struct Queue {
    Node* head;
    Node* tail;
};

#endif
