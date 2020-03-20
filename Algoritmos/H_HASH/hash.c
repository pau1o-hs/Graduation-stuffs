#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "list.h"

struct _hash{

       LIST* list[100];
       int tableSize;
       int size;
};

HASH* New_Hash(int size) {

       HASH* h = malloc(sizeof(HASH));
       h->tableSize = size;
       h->size = 0;

       for (int i = 0; i < size; i++)
              h->list[i] = New_List();

       return h;
}

int Hash_Code(int value) {

       return value % 100;
}

void Hash_Insert(HASH* h, int value) {

       List_Add(h->list[Hash_Code(value)], value);
       h->size++;

       return;
}

void Hash_Remove(HASH* h, int value) {

       List_Remove(h->list[Hash_Code(value)], value);
       h->size--;

       return;
}

int Hash_Find(HASH* h, int value) {

       if (List_Find(h->list[Hash_Code(value)], value))
              return 1;

       return 0;
}

void Hash_Print(HASH* h) {

       int flag = 0;
       for (int i = 0; i < 100; i++) {

              if (!List_Length(h->list[i])) continue;

              if (flag) printf(", ");
              List_Print(h->list[i]);

              flag = 1;
       }

       printf("\n");
}

void Destroy_Hash(HASH* h) {

       for (int i = 0; i < 100; i++)
              Destroy_List(h->list[i]);

       free(h);
       return;
}
