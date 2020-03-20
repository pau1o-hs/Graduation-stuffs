#ifndef LIST_H
#define LIST_H

typedef struct no NO;
typedef struct list LIST;

LIST* New_List();
void Destroy_List(LIST* l);
void List_Add(LIST* l, int code, char* name, int relevance, char* link, int nKey, char **key);
void List_Remove(LIST* l, int code);
void List_Print(LIST* l);
#endif
