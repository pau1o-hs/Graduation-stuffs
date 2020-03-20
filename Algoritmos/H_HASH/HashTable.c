#include <stdio.h>
#include "hash.h"

int main(int argc, char const *argv[]) {

       HASH* h = New_Hash(100);

       char c;
       int n;

       while (scanf("%c", &c) == 1 && c != 'f') {

              switch (c) {
                     case 'i':
                            scanf("%d", &n);
                            Hash_Insert(h, n);
                     break;

                     case 'b':
                            scanf("%d", &n);

                            if (Hash_Find(h, n)) printf("encontrado\n");
                            else printf("nao encontrado\n");
                     break;

                     case 'r':
                            scanf("%d", &n);
                            Hash_Remove(h, n);
                     break;

                     case 'p':
                            Hash_Print(h);
                     break;
              }
       }

       Destroy_Hash(h);
       return 0;
}
