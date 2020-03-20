#include <stdio.h>
#include "tree.h"

int main(int argc, char const *argv[]) {

       int n;
       TREE* t = New_Tree(1);

       // Tree_Breadth(t);
       // printf("\n");

       scanf("%d", &n);

       for (; n > 1; n--)
              Tree_Find(t, 0);

       printf("%d\n", Tree_Find(t, 0));
       Destroy_Tree(t);
       return 0;
}
