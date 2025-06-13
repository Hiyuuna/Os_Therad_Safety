#ifndef QTYPE_H
#define QTYPE_H

#include <mutex>

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

// 함수 선언
Queue* init(void);
void release(Queue* q);
Reply enqueue(Queue* q, Item it);
Reply dequeue(Queue* q);
Queue* range(Queue* q, Key s, Key e);
Node* nalloc(Item it);
void nfree(Node* node);

#endif
