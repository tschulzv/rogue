
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "Bandit.h"
#include "Police.h"


/*ADD HERE YOUR STRUCTURES DATA 
E.G. populateAdjacency matrix from the scenario model, etc...*/
PoliceData* createPoliceData(Game* g) {
	if (g) {
		PoliceData* m = (PoliceData*)malloc(sizeof(struct _PoliceData));
		if (m) {
			//init your struct/containers
			m->graph = NULL;
			return m;
		}
	}
	return NULL;
}
// !!!!!!!!! falta destruir el grafo
/*don't forget to free all allocated memory*/
void destroyPoliceData(PoliceData* m) {
	if (m) {
		free(m);
	}
}

BanditData* createBanditData(Game* g) {
	if (g) {
		BanditData* r = (BanditData*)malloc(sizeof(struct _BanditData));
		if (r) {
			//init your struct/containers
			r->graph = NULL;
			return r;
		}
	}
	return NULL;
}

void destroyBanditData(BanditData* r) {
	if (r) {
		free(r);
	}
}

#pragma warning(disable : 4996)
Game* createGame(char* fileName) {
	FILE* in = NULL;
	Game* g = NULL;
	char** board = NULL;
	char line[MAX_LINE_LENGTH];
	int line_count = 0;
	int N = 0;
	in = fopen(fileName, "r");

	if (!in)
	{
		printf("invalid file name %s \n",fileName);
		return NULL;
	}

	// crear la estructura game
	g = (Game*) malloc(sizeof(struct _Game));
	g->BANDIT = 'B';
	fgets(line, MAX_LINE_LENGTH, in);
	N = atoi(line); // lee el tamanho del escenario creo?
	g->N = N;

	board = (char**)malloc(N * sizeof(char*));
	for (int i = 0; i < N; i++)
		board[i] = (char*)malloc(N * sizeof(char));

	for (int i = 0; i < N; i++) {
		fgets(line, MAX_LINE_LENGTH, in);
		for (int j = 0; j < N; j++) {
			board[i][j] = line[2*j];
			// check for police's location
			if (board[i][j] == 'P') {
				g->POLICE = board[i][j];
				board[i][j] = '.';
				g->policeSite.i = i;
				g->policeSite.j = j;
			}

			// check for bandit's location
			if (board[i][j] == g->BANDIT) {
				board[i][j] = '.';
				g->banditSite.i = i;
				g->banditSite.j = j;
			}
		}
	}
	g->scenario = createScenario(board,N);
	g->policeData = createPoliceData(g);
	g->banditData = createBanditData(g);
	//dispose board allocated memory
	if (board) {
		for (int i = 0; i < N; i++)
			free(board[i]);
		free(board);
	}

	if (in)
		fclose(in);
	return g;
}

Adjacent* getNeighbors(Graph g, int i, int j) {
	if (NULL == g || i < 0 || j < 0) return NULL;
	return g[i][j].neighbors;
}

void destroyGame(Game* g) {
	if (g) {
		if (g->scenario) {
			free(g->scenario);
		}
		if (g->policeData) {
			destroyPoliceData(g->policeData);
		}
		if (g->banditData) {
			destroyBanditData(g->banditData);
		}
		free(g);
	}
}

// dado un scenario crea un grafo de N x N nodos
Graph createGraph(Scenario sce, int N) {
	if (N <= 0) return NULL;
	// creamos N filas en la matriz
	Graph g = (Graph)malloc(N * sizeof(Node*));
	if (g == NULL) return NULL;

	// recorrer los sites del scenario y crear los nodos correspondientes
	for (int i = 0; i < N; i++) {
		// creamos N columnas por cada fila
		g[i] = malloc(N * sizeof(Node));
		if (g[i] == NULL) return NULL;

		// crear los nodos para cada celda 
		for (int j = 0; j < N; j++) {
			Site thisSite = { i, j };
			// inicializar datos del nodo
			g[i][j].site = thisSite;
			g[i][j].neighbors = NULL;

			// si no es una pared
			if (isRoomScenario(&sce, thisSite) || isCorridorScenario(&sce, thisSite)) {
				// distancias (x, y) a los nodos vecinos
				int distances[8][2] = {
					{-1, 0}, {1, 0}, {0, -1}, {0, 1}, 
					{-1, -1}, {-1, 1}, {1, -1}, {1, 1} // diagnales (si es corredor no se pueden mover)
				};

				// comprobar cuales de los vecinos son movimientos validos
				for (int n = 0; n < 8; n++) {
					// hallar coordenadas del vecino y crear su site
					int xNeighbor = i + (distances[n][0]);
					int yNeighbor = j + (distances[n][1]);

					// verificar que sean coordenadas validas
					if (xNeighbor >= 0 && xNeighbor < N && yNeighbor >= 0 && yNeighbor < N) {
						Site neighborSite = { xNeighbor, yNeighbor };

						// si es un movimiento legal agregamos a su lista de adyacentes
						if (isLegalMoveScenario(&sce, thisSite, neighborSite)) {
							Adjacent *newAdj = (Adjacent*)malloc(sizeof(Adjacent));
							if (NULL == newAdj) return NULL;
							newAdj->site = neighborSite;
							newAdj->next = g[i][j].neighbors; // anhadir al inicio de la lista para evitar tener que recorrer
							g[i][j].neighbors = newAdj; // actualizar inicio de la lista de neighbors
						}
					
					}
				}
			} 

		}
	}

	return g;
}


Site getPoliceSite(Game* g) { return g->policeSite; }
Site getBanditSite(Game* g) { return g->banditSite; }
Scenario* getScenario(Game* g) { return g->scenario; }

// print representation of game state 
void printGame(Game* g) {
	for (int i = 0; i < g->N; i++) {
		for (int j = 0; j < g->N; j++) {
			Site site;
			site.i = i;
			site.j = j;
			if (equalsSite(g->banditSite, g->policeSite) && (equalsSite(g->banditSite, site))) printf("* ");
			else if (equalsSite(g->banditSite,site))                               printf("%c ",g->BANDIT);
			else if (equalsSite(g->policeSite,site))                             printf("%c ",g->POLICE);
			else if (isRoomScenario(g->scenario,site))                                 printf(". ");
			else if (isCorridorScenario(g->scenario,site))                             printf("+ ");
			else if (isRoomScenario(g->scenario,site))                                 printf(". ");
			else if (isWallScenario(g->scenario,site))                                 printf("  ");
		}
		printf("\n");
	}
	printf("------------------------------------\n");
}

void playGame(Game* g) {
	if (g) {
		for (int t = 1; 1 == 1; t++) {
			printf("Move %d\n", t);
			// bandit moves
			if (equalsSite(g->policeSite, g->banditSite)) break;
			Site next = moveBandit(g);
			if (isLegalMoveScenario(g->scenario, g->banditSite, next)) g->banditSite = next;
			else {
				printf("Bandit caught cheating\n");
				return;
			}
			printGame(g);
			// police moves
			if (equalsSite(g->policeSite, g->banditSite)) break;
			next = movePolice(g);
			if (isLegalMoveScenario(g->scenario, g->policeSite, next)) g->policeSite = next;
			else {
				printf("Police caught cheating\n");
				return;
			}
			printGame(g);
		}
		printf("Caught by police\n");
	}
}
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Way of use: c:>Proyecto4.exe scenario1.txt\n");
		return 1;
	}
	// se crea el game
	Game* g = createGame(argv[1]);
	if (NULL == g) return 1;

	// crear el grafo dado el scenario del game
	Graph graph = createGraph(*g->scenario, g->N);
	if (NULL == graph) return 1;
	
	// guardar el grafo en police data y bandit data 
	g->policeData->graph = graph;
	g->banditData->graph = graph;

	printGame(g);

	// 
	playGame(g);
	destroyGame(g);
	system("pause");
	return 0;
}

/*
// agregar a la lista de adyacentes
int addToList(Node n, Adjacent *new) {
	if (n.neighbors == NULL) { // caso lista vacia
		n.neighbors = new;
		return 1;
	} else { // recorrer la lista hasta el ultimo elemento
		Adjacent *a = n.neighbors;
		while (a->next != NULL) {
			a = a->next;
		}
		a->next = new;
		return 1;
	}
	return 0;
}*/