#ifndef LIST_H
#define LIST_H

typedef struct _node NODE;
typedef struct _list LIST;

LIST* New_List();
NODE* New_Node();

void List_Add(LIST* l, NODE* node);
void List_Reposition(LIST* l);
void List_Sort(LIST* l);
void List_Remove(LIST* l);
void List_Print(LIST* l);

void Destroy_List(LIST* l);
#endif
