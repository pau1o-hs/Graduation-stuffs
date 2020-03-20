#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct pilha {

       ITEM item[MAX];
       int topo;
};

int isNull(PILHA* p) {

       if (p != NULL)
              return 0;

       printf("Pilha não existe\n");
       return 1;
}

PILHA* Pilha_Criar() {

       PILHA* p = malloc (sizeof(PILHA));
       p->topo = 0;

       return p;
}

int Pilha_Limpar(PILHA* p, int destroyStack) {

       if (isNull(p))
              return 0;

       if (destroyStack) free(p);
       else p->topo = 0;
       return 1;
}

int Pilha_Cheia(PILHA* p) {

       if (isNull(p))
              return 0;

       if (p->topo <= MAX)
              return 0;

       printf("Pilha está cheia\n");
       return 1;
}

int Pilha_Vazia(PILHA* p) {

       if (isNull(p))
              return 0;

       if (p->topo > 0)
              return 0;

       return 1;
}

int Pilha_Topo(PILHA* p) {

       return p->topo;
}

ITEM Pilha_ItemTopo(PILHA* p) {

       if (isNull(p) || p->topo <= 0)
              return ERRO;

       return p->item[p->topo - 1];
}

int Pilha_Empilhar(PILHA* p, ITEM item) {

       if (isNull(p) || Pilha_Cheia(p))
              return 0;

       p->item[p->topo] = item;
       p->topo++;

       return 1;
}

ITEM Pilha_Desempilhar(PILHA* p) {

       if (isNull(p) || Pilha_Vazia(p))
              return ERRO;

       ITEM item;
       item = p->item[p->topo - 1];

       p->item[p->topo - 1] = ERRO;
       p->topo--;

       return item;
}

int Pilha_Imprimir(PILHA* p) {

       if (isNull(p) || Pilha_Vazia(p))
              return 0;

       for (int i = p->topo - 1; i >= 0; i--)
              printf("%c", p->item[i]);

       printf("\n");
       return 1;
}
