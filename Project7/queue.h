#ifndef QUEUE_H
#define QUEUE_H

#include "qtype.h"

Queue* init(void);
void release(Queue* q);
Reply enqueue(Queue* q, Item it);
Reply dequeue(Queue* q);
Queue* range(Queue* q, Key s, Key e);

Node* nalloc(Item it);
void nfree(Node* node);

#endif
