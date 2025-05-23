/* Circular Queue - Tania Schulz */

#include <stdlib.h>
#include "Scenario.h"

// implementacion de cola circular que almacena tipo Site
typedef struct CircQueue {
    int front, end;
    int size;
    int max;
    Site* arr;
} CircQueue;

typedef CircQueue* Queue;

Queue create(int max);

int isEmpty(Queue q);

int isFull(Queue q);

int enqueue(Queue q, Site s);

int dequeue(Queue q, Site* retval);

int peek(Queue q, Site* retval);

void destroy(Queue q);