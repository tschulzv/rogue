#include <stdlib.h>
#include "Bandit.h"
#include "Game.h"
#include "Utils.h"
#include "Queue.h"
#define MAX_DEPTH 6

// similar al algoritmo BFS usado para el policia, pero 
// en este caso hallamos todas las distancias y las guardamos 
// en una matriz
void computeDistancesFromPolice(Game* g, int** dist) {
    Graph graph = g->policeData->graph;
    Site police = getPoliceSite(g);
    Scenario* s = getScenario(g);
    int N = g->N;

    // inicializar distancias con nullSite
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            dist[i][j] = -1;

    // crear cola
    Queue q = create(N * N);
    // verificacion de creacion correcta 
    if (NULL == q) {
        freeIntMatrix(dist, N);
        return;
    }


    // insertar nodo inicial N (policia)
    enqueue(q, police);
    // poner la distancia al nodo inicial = 0
    dist[police.i][police.j] = 0;

    while (!isEmpty(q)) {
        Site current;
        dequeue(q, &current);
        int d = dist[current.i][current.j];

        Adjacent* a = graph[current.i][current.j].neighbors;
        while (a) {
            int ni = a->site.i;
            int nj = a->site.j;
            if (dist[ni][nj] == -1) {
                dist[ni][nj] = d + 1;
                enqueue(q, a->site);
            }
            a = a->next;
        }
    }
    destroy(q);
}

Site moveBandit(Game* g) {
    int N = g->N;
    Scenario* s = getScenario(g);
    Graph graph = g->banditData->graph;
    Site current = getBanditSite(g);
    Site bestMove = current;

    // --- calcular distancias desde el policía ---
    int** dist = malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++)
        dist[i] = malloc(N * sizeof(int));
    computeDistancesFromPolice(g, dist);  // función previa

    // --- estructuras BFS desde el bandido ---
    int** visited = malloc(N * sizeof(int*));
    Site** parent = malloc(N * sizeof(Site*));
    for (int i = 0; i < N; i++) {
        visited[i] = calloc(N, sizeof(int));
        parent[i] = malloc(N * sizeof(Site));
        for (int j = 0; j < N; j++) {
            parent[i][j].i = -1;
            parent[i][j].j = -1;
        }
    }

    Queue q = create(N * N);
    enqueue(q, current);
    visited[current.i][current.j] = 1;
    int depth = 0;

    // track del mejor nodo encontrado
    Site bestTarget = current;
    int maxDistance = dist[current.i][current.j];

    while (!isEmpty(q) && depth <= MAX_DEPTH) {
        int levelSize = q->size; // para procesar por niveles
        for (int k = 0; k < levelSize; k++) {
            Site node;
            dequeue(q, &node);

            if (dist[node.i][node.j] > maxDistance) {
                maxDistance = dist[node.i][node.j];
                bestTarget = node;
            }

            Adjacent* a = graph[node.i][node.j].neighbors;
            while (a) {
                int ni = a->site.i, nj = a->site.j;
                if (!visited[ni][nj]) {
                    visited[ni][nj] = 1;
                    parent[ni][nj] = node;
                    enqueue(q, a->site);
                }
                a = a->next;
            }
        }
        depth++;
    }

    // reconstruir el primer paso hacia el bestTarget
    Site step = bestTarget;
    while (!equalsSite(parent[step.i][step.j], current)) {
        step = parent[step.i][step.j];
    }
    bestMove = step;

    // liberar memoria
    destroy(q);
    for (int i = 0; i < N; i++) {
        free(dist[i]);
        free(visited[i]);
        free(parent[i]);
    }
    free(dist);
    free(visited);
    free(parent);

    return bestMove;
}
