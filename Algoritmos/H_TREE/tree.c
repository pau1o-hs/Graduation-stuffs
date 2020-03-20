#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tree.h"
#include "queue.h"

struct _tree{

       ITEM content;
       TREE* child[2];
};

TREE* New_Tree(ITEM content, int value) {

       TREE* t = malloc (sizeof(TREE));
       t->content = value;

       for (int i = 0; i < 2; i++) t->child[i] = NULL;
       return t;
}

int Tree_Insert(TREE* t, ITEM content) {

       if (content < t->content) {

              if (t->child[0] != NULL) return Tree_Insert(t->child[0], content);
              else {
                     t->child[0] = New_Tree(content);
                     return 1;
              }
       }
       else if (content > t->content) {

              if (t->child[1] != NULL) return Tree_Insert(t->child[1], content);
              else {
                     t->child[1] = New_Tree(content);
                     return 1;
              }
       }

       return 0;
}

ITEM Tree_Remove(TREE* t, ITEM content) {

       if (t == NULL) return -404;

       if (t->content == content) {

              int tmp = t->content;

              free(t);
              return tmp;
       }
       else if ((t->child[0] != NULL && t->child[0]->content == content) || (t->child[1] != NULL && t->child[1]->content == content)) {

              int child;

              if (t->child[0] != NULL && t->child[0]->content == content) child = 0;
              else child = 1;

              TREE* tmp = t->child[child];

              if (tmp->child[1] != NULL) {

                     if (tmp->child[1]->child[0] == NULL) tmp->child[1]->child[0] = tmp->child[0];
                     else tmp->child[1]->child[0]->child[0] = tmp->child[0];

                     t->child[child] = tmp->child[1];
              }
              else t->child[child] = tmp->child[0];

              int ans = tmp->content;

              free(tmp);
              return ans;
       }
       else if (content < t->content) return Tree_Remove(t->child[0], content);
       else return Tree_Remove(t->child[1], content);

}

int Tree_Find(TREE* t, ITEM content) {

       if (t == NULL) return 0;

       if (t->content == content) return 1;
       else if (t->content > content) return Tree_Find(t->child[0], content);
       else return Tree_Find(t->child[1], content);
}

void Tree_InOrder(TREE* t) {

       if (t == NULL) return;

       Tree_InOrder(t->child[0]);
       printf("%d ", t->content);
       Tree_InOrder(t->child[1]);
}

void Tree_PreOrder(TREE* t) {

       if (t == NULL) return;

       printf("%d ", t->content);
       Tree_PreOrder(t->child[0]);
       Tree_PreOrder(t->child[1]);
}

void Tree_PostOrder(TREE* t) {

       if (t == NULL) return;

       Tree_PostOrder(t->child[0]);
       Tree_PostOrder(t->child[1]);
       printf("%d ", t->content);
}

void Tree_Breadth(TREE* t) {

       QUEUE* q = New_Queue();
       Queue_Push(q, t);

       while (!Queue_Empty(q)) {

              TREE* tmp = Queue_Pop(q);
              printf("%d ", tmp->content);

              if (tmp->child[0] != NULL) Queue_Push(q, tmp->child[0]);
              if (tmp->child[1] != NULL) Queue_Push(q, tmp->child[1]);
       }

       Destroy_Queue(q);
}

void Destroy_Tree(TREE* t) {

       if (t == NULL) return;

       Destroy_Tree(t->child[0]);
       Destroy_Tree(t->child[1]);

       free(t);
}
