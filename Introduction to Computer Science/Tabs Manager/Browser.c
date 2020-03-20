#include <stdio.h>
#include "list.h"

int main(int argc, char const *argv[]) {

       LIST* l = New_List();
       int operation;

       while (scanf("%d\n", &operation) == 1 && operation != 5) {

              switch (operation) {
                     case 1:
                            List_Add(l, New_Node());
                     break;

                     case 2:
                            List_Reposition(l);
                     break;

                     case 3:
                            List_Sort(l);
                     break;

                     case 4:
                            List_Print(l);
                     break;
              }
       }

       Destroy_List(l);
       return 0;
}
