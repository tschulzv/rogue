#include <stdlib.h>
#include "Police.h"
#include "Queue.h"
#include "Game.h"
#include "Utils.h"


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
		Site nullSite = { -1, -1 }; // equivalente a retornar un NULL, para casos de error

		// ---- ALGORITMO BFS ----
		// crear matriz de visitados
		int** visited = malloc(N * sizeof(int*));
		// verificacion de memoria asignada
		if (NULL == visited) return nullSite;

		// marcar todo como no visitado
		for (int i = 0; i < N; i++) {
			visited[i] = calloc(N, sizeof(int)); 
		}
		
		// matriz de previos
		Site** prev = malloc(N * sizeof(Site*));
		if (NULL == prev) {
			freeIntMatrix(visited, N);
			return nullSite;
		}
		// inicializar con siteNull 
		for (int i = 0; i < N; i++) {
			prev[i] = malloc(N * sizeof(Site));
			for (int j = 0; j < N; j++) {
				prev[i][j].i = -1;
				prev[i][j].j = -1;
			}
		}

		// crear cola
		Queue q = create(N*N);
		// verificacion de creacion correcta 
		if (NULL == q) { 
			freeIntMatrix(visited, N);
			freeSiteMatrix(prev, N);
			return nullSite;
		}

		// insertar nodo inicial N(policia)
		enqueue(q, police); 
		// marcar nodo inicial como visitado
		visited[police.i][police.j] = 1;

		// mientras la cola no esta vacia
		while (!isEmpty(q)) {
			// desencolar un nodo 'N'
			Site node;
			dequeue(q, &node);
			// salir del bucle cuando se encuentra la posicion del bandit
			if (node.i == bandit.i && node.j == bandit.j) break;

			// obtener sus adyacentes
			Adjacent* a = getNeighbors(graph, node.i, node.j);
			// por cada adyacente 'A' de 'N':
			while (a) {
				int adjI = a->site.i;
				int adjJ = a->site.j;
				// si A no esta visitado aun 
				if (!visited[adjI][adjJ]) {
					// marcar como visitado
					visited[adjI][adjJ] = 1;
					// guardar su previo (N)
					prev[adjI][adjJ] = node;
					// encolar A
					enqueue(q, a->site);
				}
				a = a->next; // avanzar al siuiente elemento de la lista
			}
		}

		// reconstruir camino del bandit al police
		Site currentSite = bandit;
		
		// error: no se encontró camino
		if (prev[bandit.i][bandit.j].i == -1 && prev[bandit.i][bandit.j].j == -1) {
			destroy(q);
			freeIntMatrix(visited, N);
			freeSiteMatrix(prev, N);
			return nullSite;
		}

		// ir hacia atras hasta que encontremos el nodo con prev = police
		// este nodo sera el paso que daremos 
		while (!equalsSite((prev[currentSite.i][currentSite.j]), police)) {
			currentSite = prev[currentSite.i][currentSite.j];
		}
		move = currentSite; // retornar el primer paso del BFS hacia el bandit
		
		// liberar memoria
		destroy(q);
		freeIntMatrix(visited, N);
		freeSiteMatrix(prev, N);
	}
	
	return move;
} 
