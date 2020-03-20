#ifndef QUEUE_H
#define QUEUE_H
#include "tree.h"

#define MAX 10000
#define ERROR -404

typedef struct _queue QUEUE;

QUEUE* New_Queue();
void Destroy_Queue(QUEUE* q);
int Queue_Empty(QUEUE* q);
void Queue_Push(QUEUE* q, TREE* item);
TREE* Queue_Pop(QUEUE* q);
int Queue_Size(QUEUE* q);
void Queue_Print(QUEUE* q);

#endif
