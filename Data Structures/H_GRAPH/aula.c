#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

int n, m;
Graph* g;

int dist[100], prev[100];
void bfs(int s) {

        memset(dist, -1, sizeof(dist));
        memset(prev, -1, sizeof(dist));
        dist[s] = 0, prev[s] = s;

        int q[n], size = 1;
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
                                prev[adj[i]] = u;
                        }
                }
        }
}

int main(int argc, char const *argv[]) {

        int a, b;

        scanf("%d %d", &n, &m);
        g = Graph_new(n);

        for (int i = 0; i < m; i++) {
                scanf("%d %d", &a, &b);

                Graph_insertEdge(g, a-1, b-1, 1);
                Graph_insertEdge(g, b-1, a-1, 1);
        }

        int s;
        scanf("%d", &s);
        bfs(--s);

        // PRINT PATH
        for (int i = 0; i < n; i++) {

                printf("Path to %d\n", i);
                printf("Dist from %d : %d\n", i, dist[i]);
                int flag = 0;
                for (int j = i; prev[j] != j && prev[j] != -1;) {

                        flag = 1;
                        printf("%d <- ", j);
                        j = prev[j];
                }
                if (flag) printf("%d \n", s);
                printf("\n");
        }

        return 0;
}
