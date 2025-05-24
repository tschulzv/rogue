#include <stdlib.h>
#include "Police.h"
#include "Queue.h"
#include "Game.h"


//implement here the Police game stragegy, i.e. use shortest path to bandit
Site movePolice(Game* game) {
	Site move;
	move.i = 0;
	move.j = 0;
	if (game) {
		Graph graph = game->policeData->graph;
		Site police = getPoliceSite(game);
		Site bandit = getBanditSite(game);
		int N = game->N;

		// ---- ALGORITMO BFS ----
		// crear matriz de visitados
		int** visited = malloc(N * sizeof(int*));
		for (int i = 0; i < N; i++) {
			visited[i] = calloc(N, sizeof(int)); // inicializa a 0
		}
		//if (NULL == visited) return NULL;
		
		// matriz de previos
		Site** prev = malloc(N * sizeof(Site*));
		for (int i = 0; i < N; i++) {
			prev[i] = malloc(N * sizeof(Site));
			for (int j = 0; j < N; j++) {
				prev[i][j].i = -1;
				prev[i][j].j = -1;
			}
		}

		// crear cola e insertar nodo inicial N (policia)
		Queue q = create(N*N);
		enqueue(q, police); 
		// marcar nodo inicial como visitado
		visited[police.i][police.j] = 1;

		// mientras la cola no esta vacia
		while (!isEmpty(q)) {
			// desencolar un nodo 'N'
			Site node;
			dequeue(q, &node);
			// obtener sus adyacentes
			Adjacent* a = getNeighbors(graph, node.i, node.j);
			// por cada adyacente 'A' de 'N':
			// 1. marcar como visitado
			// 1. encolar
		}

		// take random legal move
		int n = 0;
		for (int i = 0; i < game->N; i++) {
			for (int j = 0; j < game->N; j++) {
				//Site site = new Site(i, j);
				Site site;
				site.i = i;
				site.j = j;
				if (isLegalMoveScenario(game->scenario, police, site)) {
					n++;
					if (rand() % 1000 <= 1000 / n) {
						move.i = site.i;
						move.j = site.j;
					}
				}
			}
		}
		for (int i = 0; i < N; i++) free(visited[i]);
		free(visited);
	}
	
	return move;
} 