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

Queue create(int max) {
    Queue q = (Queue)malloc(sizeof(CircQueue));
    if (NULL == q) return NULL;

    Site* arr = (Site*)malloc(sizeof(Site) * max);
    if (NULL == arr) return NULL;

    q->arr = arr;
    q->max = max;
    q->size = 0;
    q->front = 0;
    q->end = max - 1;
    return q;
}

int isEmpty(Queue q) {
    if (NULL == q) return INT_MIN;
    return (q->size == 0);
}

int isFull(Queue q) {
    if (q == NULL) return INT_MIN;
    return (q->size == q->max);
}

/*
int isFull(Queue q) {
    if (NULL == q) return INT_MIN;
    return (q->size == q->max - 1); // one space is left unused
}
*/

int enqueue(Queue q, Site s) {
    if (NULL == q) return 0;

    if (!isFull(q)) {
        q->end = (q->end + 1) % q->max;
        q->arr[q->end] = s;
        q->size++;
        return 1;
    }
    return 0;
}

int dequeue(Queue q, Site* retval) {
    if (NULL == q) return 0;
    if (!isEmpty(q)) {
        *retval = q->arr[q->front];
        q->front = (q->front + 1) % q->max;
        q->size--;
        return 1;
    }
    return 0;
}

int peek(Queue q, Site* retval) {
    if (NULL == q) return 0;
    if (!isEmpty(q)) {
        *retval = q->arr[q->front];
        return 1;
    }
    return 0;
}

void destroy(Queue q) {
    if (q != NULL) {
        free(q->arr);
        free(q);
    }
}