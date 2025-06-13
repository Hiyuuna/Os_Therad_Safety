#include "queue.h"
#include <mutex>

using namespace std;

mutex qmtx;

Node* nalloc(Item it) {
    Node* n = new Node;
    n->item = it;
    n->next = nullptr;
    return n;
}

void nfree(Node* node) {
    delete node;
}

Queue* init(void) {
    Queue* q = new Queue;
    q->head = nullptr;
    q->tail = nullptr;
    return q;
}

void release(Queue* q) {
    Node* ptr = q->head;
    while (ptr != nullptr) {
        Node* tmp = ptr->next;
        nfree(ptr);
        ptr = tmp;
    }
    delete q;
}

Reply enqueue(Queue* q, Item it) {
    lock_guard<mutex> lk(qmtx);
    Reply rp{ false, {} };

    Node* n = nalloc(it);
    if (n == nullptr) return rp;

    if (q->head == nullptr) {
        q->head = q->tail = n;
    }
    else {
        Node* prev = nullptr;
        Node* curr = q->head;

        while (curr != nullptr && curr->item.key <= it.key) {
            prev = curr;
            curr = curr->next;
        }

        if (prev == nullptr) {
            n->next = q->head;
            q->head = n;
        }
        else {
            prev->next = n;
            n->next = curr;
            if (curr == nullptr) q->tail = n;
        }
    }

    rp.success = true;
    rp.item = it;
    return rp;
}

Reply dequeue(Queue* q) {
    lock_guard<mutex> lk(qmtx);
    Reply rp{ false, {} };

    if (q->head == nullptr) return rp;

    Node* del = q->head;
    q->head = del->next;
    if (q->head == nullptr) q->tail = nullptr;

    rp.success = true;
    rp.item = del->item;
    nfree(del);
    return rp;
}

Queue* range(Queue* q, Key s, Key e) {
    lock_guard<mutex> lk(qmtx);
    Queue* res = init();
    Node* p = q->head;

    while (p != nullptr) {
        if (p->item.key >= s && p->item.key <= e) {
            enqueue(res, p->item);
        }
        p = p->next;
    }

    return res;
}
