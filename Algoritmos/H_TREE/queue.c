#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "tree.h"

struct _queue {

       TREE* item[MAX];
       int size;
};

QUEUE* New_Queue() {

       QUEUE* q = malloc(sizeof(QUEUE));
       q->size = 0;

       return q;
}

void Destroy_Queue(QUEUE* q) {

       free(q);
       return;
}

int Queue_Empty(QUEUE* q) {

       if (q->size == 0) return 1;
       return 0;
}

void Queue_Push(QUEUE* q, TREE* item) {

       if (q->size + 1 == MAX) return;

       q->item[q->size] = item;
       q->size++;

       return;
}

TREE* Queue_Pop(QUEUE* q) {

       if (Queue_Empty(q)) return NULL;

       TREE* item = q->item[0];

       for (int i = 0; i < q->size - 1; i++)
              q->item[i] = q->item[i + 1];

       q->size--;
       return item;
}

int Queue_Size(QUEUE* q) {

       return q->size;
}
