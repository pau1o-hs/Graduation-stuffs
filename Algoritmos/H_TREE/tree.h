#ifndef TREE_H
#define TREE_H

#define ITEM int

typedef struct _tree TREE;

TREE* New_Tree(ITEM content);

int Tree_Insert(TREE* t, ITEM content);
int Tree_Find(TREE* t, ITEM content);
int Tree_Remove(TREE* t, ITEM content);

void Tree_InOrder(TREE* t);
void Tree_PreOrder(TREE* t);
void Tree_PostOrder(TREE* t);
void Tree_Breadth(TREE* t);

void Destroy_Tree(TREE* t);
#endif
