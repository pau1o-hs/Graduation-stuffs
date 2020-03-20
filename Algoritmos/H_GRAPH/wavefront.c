#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

#define N 9
#define M 6

Graph* g;

int dist[N];
void bfs(int s) {

        memset(dist, -1, sizeof(dist));
        dist[s] = 0;

        int q[N], size = 1;
        q[0] = s;

        while (size) {

                int u = q[0];
                for (int i = 0; i < size; i++)
                        q[i] = q[i + 1];
                size--;

                int* adj = Graph_getAdjacents(g, u);
                for (int i = 0; i < Graph_getDegree(g, u); i++) {

                        if (dist[adj[i]] == -1) {
                                dist[adj[i]] = dist[u] + 1;
                                q[size++] = adj[i];
                        }
                }
        }
}

int main(int argc, char const *argv[]) {

        g = Graph_new(N);

        Graph_insertEdge(g, 0, 1, 1);
        Graph_insertEdge(g, 1, 0, 1);

        Graph_insertEdge(g, 0, 3, 1);
        Graph_insertEdge(g, 3, 0, 1);

        Graph_insertEdge(g, 1, 2, 1);
        Graph_insertEdge(g, 2, 1, 1);

        Graph_insertEdge(g, 4, 1, 1);
        Graph_insertEdge(g, 1, 4, 1);

        Graph_insertEdge(g, 3, 4, 1);
        Graph_insertEdge(g, 4, 3, 1);

        Graph_insertEdge(g, 4, 5, 1);
        Graph_insertEdge(g, 5, 4, 1);

        Graph_insertEdge(g, 3, 6, 1);
        Graph_insertEdge(g, 6, 3, 1);

        Graph_insertEdge(g, 4, 7, 1);
        Graph_insertEdge(g, 7, 4, 1);

        Graph_insertEdge(g, 5, 8, 1);
        Graph_insertEdge(g, 8, 5, 1);

        Graph_insertEdge(g, 6, 7, 1);
        Graph_insertEdge(g, 7, 6, 1);

        Graph_insertEdge(g, 7, 8, 1);
        Graph_insertEdge(g, 8, 7, 1);

        bfs(4);

        for (int i = 0; i < N; i++) {
                printf("%d : %d\n", i, dist[i]);
        }
        return 0;
}
