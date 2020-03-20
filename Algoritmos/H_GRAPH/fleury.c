#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define N 5
#define M 6

Graph* g;
int vis[N];

void dfs(int s) {

        vis[s] = 1;

        int* adj = Graph_getAdjacents(g, s);
        for (int i = 0; i < Graph_getDegree(g, s); i++) {

                if (!vis[adj[i]]) dfs(adj[i]);
        }

        return;
}

int main(int argc, char const *argv[]) {

        g = Graph_new(N);

        Graph_insertEdge(g, 0, 1, 1);
        Graph_insertEdge(g, 1, 0, 1);

        Graph_insertEdge(g, 1, 3, 1);
        Graph_insertEdge(g, 3, 1, 1);

        Graph_insertEdge(g, 3, 4, 1);
        Graph_insertEdge(g, 4, 3, 1);

        Graph_insertEdge(g, 4, 1, 1);
        Graph_insertEdge(g, 1, 4, 1);

        Graph_insertEdge(g, 1, 2, 1);
        Graph_insertEdge(g, 2, 1, 1);

        Graph_insertEdge(g, 0, 2, 1);
        Graph_insertEdge(g, 2, 0, 1);

        for (int i = 0; i < N; i++) {
                int d = Graph_getDegree(g, i);
                if (d & 1) return printf("Não há ciclo\n"), 0;
        }

        int ans[M + 1], count = 1, current = 0;
        ans[0] = 0;

        while (count <= M) {

                int* adj = Graph_getAdjacents(g, current);

                for (int j = 0; j < Graph_getDegree(g, current); j++) {

                        for (int k = 0; k < N; k++)
                                vis[k] = 0;

                        int tmp = adj[j];

                        Graph_eraseEdge(g, current, tmp);
                        Graph_eraseEdge(g, tmp, current);
                        dfs(tmp);

                        if (vis[tmp] || Graph_getDegree(g, current) == 1) {
                                current = ans[count++] = tmp;
                                break;
                        }

                        Graph_insertEdge(g, current, tmp, 1);
                        Graph_insertEdge(g, tmp, current, 1);
                }

                free(adj);
        }

        printf("Path: ");
        for (int i = 0; i < M + 1; i++)
                printf("%d ", ans[i]);
        printf("\n");
        return 0;
}
