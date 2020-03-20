#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "list.h"

struct _node {

       char title[50];
       char url[50];
       int date[2];
       int hour[2];

       int order;

       NODE* prev;
       NODE* next;
};

struct _list {

       NODE* start;
       NODE* end;

       int size;
};

LIST* New_List() {

       LIST* l = malloc (sizeof(LIST));
       l->start = NULL;
       l->end = NULL;
       l->size = 0;

       return l;
}

NODE* New_Node() {

       NODE* node = malloc(sizeof(NODE));

       scanf("%[a-zA-Z0-9- ]\n", node->title);
       scanf("%s\n", node->url);
       scanf("%d %d\n", &node->date[0], &node->date[1]);
       scanf("%d %d\n", &node->hour[0], &node->hour[1]);

       char tmp[10];
       sprintf (tmp, "%02d%02d%02d%02d", node->date[1], node->date[0], node->hour[0], node->hour[1]);

       node->order = atoi(tmp);

       node->prev = NULL;
       node->next = NULL;
       return node;
}

void List_Add(LIST* l, NODE* node) {

       l->size++;

       if (l->end == NULL) {

              l->start = node;
              l->end = node;
              return;
       }

       node->prev = l->end;

       l->end->next = node;
       l->end = l->end->next;
       return;
}

void List_Reposition(LIST* l) {

       char title[50];
       int index;

       scanf("%[a-zA-Z0-9- ]\n", title);
       scanf("%d\n", &index);

       NODE* target = l->start;
       NODE* it = l->start;

       //GET TARGET NODE
       while (true) {

              if (target == NULL) return;
              if (strcmp(target->title, title) == 0) break;
              target = target->next;
       }

       //ADJUST NODES AROUND TARGET
       if (target->prev != NULL) target->prev->next = target->next;
       if (target->next != NULL) target->next->prev = target->prev;

       if (target == l->start) l->start = target->next;
       if (target == l->end) l->end = target->prev;

       int i = 1;

       //REPOSITION
       while (true) {

              if (it == NULL) {

                     target->prev = l->end;
                     target->next = NULL;

                     l->end->next = target;
                     l->end = target;
                     break;
              }

              if (i == index) {

                     if (it->prev != NULL) it->prev->next = target;
                     target->next = it;
                     it->prev = target;

                     if (it == l->start) l->start = target;
                     if (it == l->end) l->end = target;
                     break;
              }

              it = it->next;
              i++;
       }

       return;
}

void List_Sort(LIST* l) {

       int i = 0, expo = 1;
       int greater = l->start->order;

       NODE* it = l->start;
       NODE** v = malloc (l->size * sizeof(NODE*));

       for (i = 0; i < l->size; i++, it = it->next)
              if (it->order > greater) greater = it->order;

       while (greater / expo > 0) {

              it = l->start;
              int bucket[10] = { 0 };

              for (i = 0; i < l->size; i++, it = it->next) bucket[(it->order / expo) % 10]++;
              for (i = 1; i < 10; i++) bucket[i] += bucket[i - 1];
              for (i = l->size - 1, it = l->end; i >= 0; i--, it = it->prev) v[--bucket[(it->order / expo) % 10]] = it;

              for (i = 0; i < l->size; i++) {

                     if (!i) {

                            l->start = v[0];
                            l->end = v[0];
                            v[0]->prev = NULL;
                            v[0]->next = NULL;
                            continue;
                     }

                     v[i]->prev = l->end;
                     v[i]->next = NULL;

                     l->end->next = v[i];
                     l->end = v[i];
              }

              expo *= 10;
       }

       free(v);
       return;
}

void List_Remove(LIST* l) {

       return;
}

void List_Print(LIST* l) {

       NODE* it = l->start;

       while (it != NULL) {

              printf("%s %s %02d/%02d %02d:%02d\n", it->title, it->url, it->date[0], it->date[1], it->hour[0], it->hour[1]);
              it = it->next;
       }

       printf("\n");
       return;
}

void Destroy_List(LIST* l) {

       NODE* it = l->start;
       while (it != NULL) {

              NODE* tmp_Item = it;
              it = it->next;

              free(tmp_Item);
       }

       free(l);
       return;
}
