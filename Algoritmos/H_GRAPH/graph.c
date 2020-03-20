#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

struct vertex {
        int index, value, degree;
        Edge *start;
};

struct edge {
        int weight;

        Vertex* vert;
        Edge* next;
};

struct graph {
        Vertex* vert;
        int nVertex;
};

Graph* Graph_new(int n) {

        Graph* g = malloc(sizeof(Graph));
        g->vert = malloc(sizeof(Vertex) * n);
        g->nVertex = n;

        for (int i = 0; i < n; i++) {
                g->vert[i].index = i;
                g->vert[i].value = 0;
                g->vert[i].degree = 0;
                g->vert[i].start = NULL;
        }

        return g;
}

void Graph_insertEdge(Graph* g, int a, int b, int w) {

        Edge* it = g->vert[a].start;
        g->vert[a].degree++;

        if (it == NULL) {
                g->vert[a].start = malloc(sizeof(Edge));
                g->vert[a].start->vert = &g->vert[b];
                g->vert[a].start->weight = w;
                g->vert[a].start->next = NULL;
                return;
        }
        else while (it->next != NULL) it = it->next;

        it->next = malloc(sizeof(Edge));
        it->next->vert = &g->vert[b];
        it->next->weight = w;
        it->next->next = NULL;

        return;
}

void Graph_eraseEdge(Graph* g, int a, int b) {

        Edge* toErase;
        Edge* it = g->vert[a].start;
        g->vert[a].degree--;

        if (it->vert == &g->vert[b]) {

                g->vert[a].start = it->next;
                return free(it);
        }
        else while (it->next->vert != &g->vert[b]) it = it->next;

        toErase = it->next;
        it->next = toErase->next;
        return free(toErase);
}

int Graph_getValue(Graph* g, int a) {
        return g->vert[a].value;
}

void Graph_setValue(Graph* g, int a, int value) {
        g->vert[a].value = value;
        return;
}

int Graph_getDegree(Graph* g, int a) {
        return g->vert[a].degree;
}

int* Graph_getAdjacents(Graph* g, int a) {

        if (!g->vert[a].degree) return NULL;

        int* adj = malloc(g->vert[a].degree * sizeof(int));
        Edge* it = g->vert[a].start;

        for (int i = 0; i < g->vert[a].degree; i++, it = it->next)
                adj[i] = it->vert->index;

        return adj;
}

void Graph_print(Graph* g) {

        for (int i = 0; i < g->nVertex; i++)
                printf("%d : %d\n", i, g->vert[i].value);

        return;
}
