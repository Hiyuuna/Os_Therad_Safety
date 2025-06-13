#include "qtype.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>

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
    if (!n) return rp;

    if (q->head == nullptr) {
        q->head = q->tail = n;
    }
    else {
        Node* prev = nullptr;
        Node* curr = q->head;
        while (curr && curr->item.key <= it.key) {
            prev = curr;
            curr = curr->next;
        }

        if (!prev) {
            n->next = q->head;
            q->head = n;
        }
        else {
            prev->next = n;
            n->next = curr;
            if (!curr) q->tail = n;
        }
    }

    rp.success = true;
    rp.item = it;
    return rp;
}

Reply dequeue(Queue* q) {
    lock_guard<mutex> lk(qmtx);
    Reply rp{ false, {} };

    if (!q->head) return rp;

    Node* del = q->head;
    q->head = del->next;
    if (!q->head) q->tail = nullptr;

    rp.success = true;
    rp.item = del->item;
    nfree(del);
    return rp;
}

Queue* range(Queue* q, Key s, Key e) {
    lock_guard<mutex> lk(qmtx);
    Queue* res = init();
    Node* p = q->head;

    while (p) {
        if (p->item.key >= s && p->item.key <= e) {
            enqueue(res, p->item);
        }
        p = p->next;
    }

    return res;
}

void producer(Queue* q, int id) {
    for (int i = 0; i < 3; ++i) {
        Item it;
        it.key = rand() % 500;
        it.value = id * 10 + i;
        enqueue(q, it);
    }
}

void consumer(Queue* q, int id) {
    for (int i = 0; i < 3; ++i) {
        Reply r = dequeue(q);
        if (r.success)
            cout << "[T" << id << "] Got: " << r.item.key << ", " << r.item.value << endl;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    Queue* q = init();

    thread t1(producer, q, 1);
    thread t2(producer, q, 2);
    thread t3(consumer, q, 3);
    thread t4(consumer, q, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    Queue* r = range(q, 100, 300);
    cout << "--- range ---" << endl;
    Reply x;
    while ((x = dequeue(r)).success)
        cout << x.item.key << ", " << x.item.value << endl;

    release(q);
    release(r);
    return 0;
}
