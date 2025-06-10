	
#pragma once
typedef int key;
typedef int value;

typedef struct {
	Key key;
	Value value;
}Item;


typedef struct node_T {
	Item item;
	strunct node_T* next;
}Node;

typedef struct {
	Node* head;
	Node* tail;
}Queue;

typedef struct {
	bool sucess;
	Item item;
}Reply;