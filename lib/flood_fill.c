#include "flood_fill.h"
#include <stdlib.h>
#include <stdbool.h>

// Point structure for managing pixel coordinates
typedef struct {
    int x;
    int y;
} FloodFillPoint;

// Function to check if a pixel is valid for filling
static bool is_valid_pixel(Image *src, int x, int y, Color targetColor, Color fillColor) {
    if (x < 0 || x >= src->cols || y < 0 || y >= src->rows) {
        return false;
    }
    Color currentColor = image_getColor(src, y, x);
    return (currentColor.c[0] == targetColor.c[0] && 
            currentColor.c[1] == targetColor.c[1] &&
            currentColor.c[2] == targetColor.c[2] &&
            !(currentColor.c[0] == fillColor.c[0] &&
              currentColor.c[1] == fillColor.c[1] &&
              currentColor.c[2] == fillColor.c[2]));
}

// Queue node structure for managing points in the flood-fill algorithm
typedef struct QueueNode {
    FloodFillPoint pt;
    struct QueueNode *next;
} QueueNode;

// Queue structure for the flood-fill algorithm
typedef struct {
    QueueNode *front, *rear;
} Queue;

// Function to create a new queue node
static QueueNode* new_node(int x, int y) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->pt.x = x;
    temp->pt.y = y;
    temp->next = NULL;
    return temp;
}

// Function to create an empty queue
static Queue* create_queue(void) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// Function to add a point to the queue
static void enqueue(Queue* q, int x, int y) {
    QueueNode* temp = new_node(x, y);
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

// Function to remove a point from the queue
static FloodFillPoint dequeue(Queue* q) {
    if (q->front == NULL)
        exit(EXIT_FAILURE);

    QueueNode* temp = q->front;
    FloodFillPoint pt = temp->pt;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
    return pt;
}

// Function to check if the queue is empty
static bool is_empty(Queue* q) {
    return q->front == NULL;
}

// Flood-fill algorithm
void flood_fill(Image *src, int x, int y, Color fillColor) {
    Color targetColor = image_getColor(src, y, x);
    if (targetColor.c[0] == fillColor.c[0] && 
        targetColor.c[1] == fillColor.c[1] && 
        targetColor.c[2] == fillColor.c[2]) {
        return;
    }

    Queue* q = create_queue();
    enqueue(q, x, y);

    while (!is_empty(q)) {
        FloodFillPoint pt = dequeue(q);
        int curr_x = pt.x;
        int curr_y = pt.y;

        if (is_valid_pixel(src, curr_x, curr_y, targetColor, fillColor)) {
            image_setColor(src, curr_y, curr_x, fillColor);

            enqueue(q, curr_x + 1, curr_y);
            enqueue(q, curr_x - 1, curr_y);
            enqueue(q, curr_x, curr_y + 1);
            enqueue(q, curr_x, curr_y - 1);
        }
    }

    // Free the queue
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}
