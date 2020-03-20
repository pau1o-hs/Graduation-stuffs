#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

struct node {

       int value;

       NODE* next;
       NODE* prev;
};

struct list {

       NODE* start;
       NODE* end;

       int length;
};

LIST* New_List() {

       LIST* l = malloc (sizeof(LIST));
       l->start = NULL;
       l->end = NULL;

       return l;
}

void List_Add(LIST* l, int value) {

       NODE* new_Item = malloc (sizeof(NODE));
       new_Item->value = value;

       NODE* it = l->start;

       if (l->start == NULL) {

              l->start = new_Item;
              l->end = new_Item;

              new_Item->next = NULL;
              new_Item->prev = NULL;
       }
       else while (it != NULL) {

              if (new_Item->value < it->value) {

                     if (it->prev != NULL) it->prev->next = new_Item;
                     else l->start = new_Item;

                     new_Item->next = it;
                     it->prev = new_Item;

                     break;
              }

              it = it->next;
       }

       l->length++;
       return;
}

int List_Find(LIST* l, int value) {

       NODE* it = l->start;

       while (it != NULL) {

              if (it->value == value) return 1;
              it = it->next;
       }

       return 0;
}

int List_Length(LIST* l) {

       return l->length;
}

void List_Remove(LIST* l, int value) {

       NODE* it = l->start;

       while (it != NULL) {

              if (it->value == value) {

                     if (it == l->start) l->start = it->next;
                     if (it == l->end) l->end = it->prev;

                     if (it->prev != NULL) it->prev->next = it->next;
                     if (it->next != NULL) it->next->prev = it->prev;

                     free(it);
                     break;
              }

              it = it->next;
       }

       l->length--;
       return;
}

void List_Print(LIST* l) {

       if (l->length > 1) {

              printf("(%d", l->start->value);

              NODE* it = l->start->next;
              while (it != NULL) {

                     printf(", %d", it->value);
                     it = it->next;
              }

              printf(")");
       }
       else printf("%d", l->start->value);

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
