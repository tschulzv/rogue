#include <stdlib.h>
#include "Game.h"
#include "Utils.h"


// liberar matriz de sites
void freeSiteMatrix(Site** matrix, int rows) {
	if (!matrix) return;
	for (int i = 0; i < rows; i++) {
		free(matrix[i]);
	}
	free(matrix);
}
// liberar matriz de ints
void freeIntMatrix(int** matrix, int rows) {
	if (!matrix) return;
	for (int i = 0; i < rows; i++) {
		free(matrix[i]);
	}
	free(matrix);
}
