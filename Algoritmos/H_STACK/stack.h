#ifndef STACK_H
#define STACK_H

#define ITEM char*
#define MAX 100
#define ERRO "#"

typedef struct pilha PILHA;

PILHA* Pilha_Criar();
int Pilha_Limpar(PILHA* p);
int Pilha_Cheia();
int Pilha_Vazia();
int Pilha_Topo();
ITEM Pilha_ItemTopo(PILHA* p);
int Pilha_Empilhar(PILHA* p, ITEM item);
ITEM Pilha_Desempilhar(PILHA* p);
int Pilha_Imprimir(PILHA* p);

#endif
