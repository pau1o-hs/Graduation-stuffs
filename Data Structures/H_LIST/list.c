#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct no {

       NO* item;

       NO* prev;
       NO* next;
};

struct list {

       NO* start;
       NO* end;
};

LIST* New_List() {

       LIST* l = malloc (sizeof(LIST));
       l->start = NULL;
       l->end = NULL;

       return l;
}

void Destroy_List(LIST* l) {

       NO* it = l->start;
       while (it != NULL) {

              NO* tmp_Item = it;
              it = it->next;

              free(tmp_Item);
       }

       free(l);
       return;
}

void List_Add(LIST* l, int code, char* name, int relevance, char* link, int nKey, char **key) {

       NO* new_Item = malloc (sizeof(NO));

       new_Item->code = code;
       strcpy(new_Item->name, name);
       new_Item->relevance = relevance;
       strcpy(new_Item->link, link);

       new_Item->nKey = nKey;
       for (int i = 0; i < nKey; i++)
              strcpy(new_Item->key[i], key[i]);

       new_Item->next = NULL;

       if (l->start == NULL) {
              l->start = new_Item;
              l->end = new_Item;
              return;
       }

       NO *it_Item = l->start, *tmp_Item;
       if (it_Item->code > new_Item->code) {

              tmp_Item = it_Item;
              l->start = new_Item;
              new_Item->next = tmp_Item;

              return;
       }

       while (1) {

              if (it_Item->next == NULL) {

                     it_Item->next = new_Item;
                     l->end = new_Item;
                     break;
              }

              if (it_Item->next->code > new_Item->code) {

                     tmp_Item = it_Item->next;
                     it_Item->next = new_Item;
                     new_Item->next = tmp_Item;

                     break;
              }

              it_Item = it_Item->next;
       }

       return;
}

void List_Remove(LIST* l, int code) {

       NO* it = l->start;

       if (l->start->code == code) {
              NO* tmp_Item = it->next;
              l->start = tmp_Item;

              free(it);
              return;
       }

       while (1) {

              if (it->next == NULL) {

                     if (it->code == code)
                            free(it);
                     else printf("Código não encontrado\n");
                     break;
              }

              if (it->next->code == code) {

                     NO* tmp_Item = it->next;
                     it->next = tmp_Item->next;

                     free(tmp_Item);
                     break;
              }

              it = it->next;
       }

       return;
}

void List_Print(LIST* l) {

       NO* it = l->start;

       printf(ANSI_COLOR_GREEN "-------------------------------------------------Itens atuais-------------------------------------------------" ANSI_COLOR_RESET "\n");
       while (it != NULL) {

              printf(ANSI_COLOR_BLUE "|[CODE]" ANSI_COLOR_RESET " %04d\n", it->code);
              printf(ANSI_COLOR_BLUE "|[NAME]" ANSI_COLOR_RESET " %s\n", it->name);
              printf(ANSI_COLOR_BLUE "|[RELEVANCE]" ANSI_COLOR_RESET " %04d\n", it->relevance);
              printf(ANSI_COLOR_BLUE "|[LINK]" ANSI_COLOR_RESET " %s\n", it->link);
              printf(ANSI_COLOR_BLUE "|[KEY-WORDS]" ANSI_COLOR_RESET);

              for (int i = 0; i < it->nKey; i++)
                     printf("\n%s", it->key[i]);

              printf("\n\n");

              it = it->next;
       }
       printf(ANSI_COLOR_GREEN "--------------------------------------------------------------------------------------------------------------" ANSI_COLOR_RESET "\n");

       return;
}
