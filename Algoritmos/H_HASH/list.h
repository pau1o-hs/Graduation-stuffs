#ifndef LIST_H
#define LIST_H

typedef struct node NODE;
typedef struct list LIST;

LIST* New_List();

void List_Add(LIST* l, int value);
int List_Find(LIST* l, int value);
int List_Length(LIST* l);
void List_Remove(LIST* l, int value);
void List_Print(LIST* l);

void Destroy_List(LIST* l);
#endif
