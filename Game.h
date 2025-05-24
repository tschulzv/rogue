#pragma once
#ifndef DEFINE_GAME_H
#define DEFINE_GAME_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "Scenario.h"
#define MAX_LINE_LENGTH 80


// estructura que representa a un adyacente de un nodo
typedef struct _Adjacent {
	Site site;
	struct _Adjacent* next; // apunta al siguente en la lista
} Adjacent;

// estructura que representa a un nodo del grafo
typedef struct _Node {
	Site site; // site del nodo
	Adjacent* neighbors; // lista de adyacentes (*SOLO GUARDAR MOVIMIENTOS VALIDOS*)
}Node;

typedef Node** Graph; // grafo - matriz de Node

typedef struct _PoliceData {
	Graph graph;
}PoliceData;

typedef struct _BanditData {
	Graph graph;
} BanditData;



typedef struct _Game {
	Scenario* scenario;
	char POLICE;
	char BANDIT;
	int N;
	Site policeSite;
	Site banditSite;
	PoliceData* policeData;
	BanditData* banditData;
}Game;

Game* createGame(char* fileName);
void destroyGame(Game* g);
Site getPoliceSite(Game* g);
Site getBanditSite(Game* g);
Scenario* getScenario(Game* g);
void printGame(Game* g);
void playGame(Game* g);

PoliceData* createPoliceData(Game* g);
void destroyPoliceData(PoliceData* r);

BanditData* createBanditData(Game* g);
void destroyBanditData(BanditData* r);

// funciones anhadidas
Adjacent* getNeighbors(Graph g, int i, int j);

#endif