
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "Bandit.h"
#include "Police.h"

typedef struct _Adjacent {
	int i, j; // site
	struct _Adjacent* next; // apunta al siguente en la lista
} Adjacent;


typedef struct _Node {
	int i, j; // site del nodo
	Adjacent* neighbors; // lista de adyacentes (*SOLO MOVIMIENTOS VALIDOS*)
}Node;

typedef Node **Graph; // grafo - matriz de Nodos

/*ADD HERE YOUR STRUCTURES DATA 
E.G. populateAdjacency matrix from the scenario model, etc...*/
PoliceData* createPoliceData(Game* g) {
	if (g) {
		PoliceData* m = (PoliceData*)malloc(sizeof(struct _PoliceData));
		if (m) {
			//init your struct/containers
			m->replaceThisWithYourDataContainer = NULL;
			return m;
		}
	}
	return NULL;
}
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
			r->replaceThisWithYourDataContainer = NULL;
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
			Site s = { i, j };
			// si no es una pared
			if (isRoomScenario(&sce, s) || isCorridorScenario(&sce, s)) {
				g[i][j].i = i;
				g[i][j].j = j;
			}

			// distancias (x, y) a los adyacentes
			int neighbors[8][2] = {
				{-1, 0}, {1, 0}, {0, -1}, {0, 1}, 
				{-1, -1}, {-1, 1}, {1, -1}, {1, 1} // diagnales (si es corredor no se puede mover aca)
			};

			// comprobar cuales de los adyacentes son movimientos validos
			for (int n = 0; n < 8; n++) {
				
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
	Game* g = createGame(argv[1]);
	printGame(g);
	playGame(g);
	destroyGame(g);
	return 0;
}