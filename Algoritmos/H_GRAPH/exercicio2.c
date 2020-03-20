#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

int n, m;
Graph* g;

int vis[100];

void dfs(int s) {

        vis[s]++;

        int* adj = Graph_getAdjacents(g, s);
        for (int i = 0; i < Graph_getDegree(g, s); i++) {

                if (!vis[adj[i]]) dfs(adj[i]);
        }

        return;
}

int main(int argc, char const *argv[]) {

        int a, b;

        scanf("%d %d", &n, &m);
        g = Graph_new(n);

        for (int i = 0; i < m; i++) {
                scanf("%d %d", &a, &b);

                Graph_insertEdge(g, a-1, b-1, 1);
                //Graph_insertEdge(g, b-1, a-1, 1);
        }

        int s;
        scanf("%d", &s);
        dfs(--s);

        for (int i = 0; i < m; i++) {

                int* adj = Graph_getAdjacents(g, i);

                for (int j = 0; j < Graph_getDegree(g, i); j++) {

                        int tmp = adj[j];

                        Graph_eraseEdge(g, i, tmp);
                        Graph_eraseEdge(g, tmp, i);
                        dfs(tmp);
                        Graph_insertEdge(g, i, tmp, 1);
                        Graph_insertEdge(g, tmp, i, 1);
                }

                free(adj);
        }

        int best = 0, max = vis[0];
        for (int i = 1; i < n; i++) {
                if (vis[i] > max) best = i, max = vis[i];
        }

        printf("%d", best);
        return 0;
}
