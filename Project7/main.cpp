#include "queue.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

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
