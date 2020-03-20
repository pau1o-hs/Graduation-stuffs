#ifndef GRAPH_H
#define GRAPH_H

#define MAX 100

typedef int elem;
typedef struct graph Graph;
typedef struct edge Edge;
typedef struct vertex Vertex;

Graph* Graph_new(int n);
void Graph_insertEdge(Graph* g, int a, int b, int w);
void Graph_eraseEdge(Graph* g, int a, int b);
void Graph_setValue(Graph* g, int a, int value);
int Graph_getValue(Graph* g, int a);
int Graph_getDegree(Graph* g, int a);
int* Graph_getAdjacents(Graph* g, int a);
void Graph_print(Graph* g);
#endif
