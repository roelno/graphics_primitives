#include <stdlib.h>
#include "../include/polygon.h"
#include "../include/line.h"

void *polygon_create() {
    Polygon *p = malloc(sizeof(Polygon));
    if (p == NULL) {
        return NULL;
    }

    p->oneSided = 0;
    p->nVertex = 0;
    p->vertex = NULL;
    return p;
}

void *polygon_createp(int numV, Point *vlist) {
    Polygon *p = malloc(sizeof(Polygon));
    if (p == NULL) {
        return NULL;
    }

    p->oneSided = 0;
    p->nVertex = numV;
    p->vertex = malloc(sizeof(Point) * numV);
    if (p->vertex == NULL) {
        free(p); // Free polygon if vertex allocation fails
        return NULL;
    }
    for (int i = 0; i < numV; i++) {
        point_copy(&p->vertex[i], &vlist[i]); // Copy each point
    }
    return p;
}

void polygon_free(Polygon *p) {
    if (p != NULL) {
        free(p->vertex); // Free the vertex array if it exists
        free(p); // Free the polygon structure itself
    }
}

void polygon_init(Polygon *p) {
    if (p != NULL) {
        p->oneSided = 0;
        p->nVertex = 0;
        free(p->vertex); // Free existing vertices if any
        p->vertex = NULL; // Set vertex pointer to NULL
    }
}

void polygon_set(Polygon *p, int numV, Point *vlist) {
    if (p == NULL) {
        return;
    }

    free(p->vertex);
    p->vertex = malloc(sizeof(Point) * numV);
    if (p->vertex == NULL) {
        p->nVertex = 0; // Ensure consistent state on allocation failure
        return;
    }
    for (int i = 0; i < numV; i++) {
        point_copy(&p->vertex[i], &vlist[i]); // Copy new vertices
    }
    p->nVertex = numV; // Update vertex count
}

void polygon_clear(Polygon *p) {
    if (p != NULL) {
        free(p->vertex); // Free the vertex array
        p->vertex = NULL; // Reset vertex pointer
        p->nVertex = 0;
        p->oneSided = 0;
    }
}

void polygon_setSided(Polygon *p, int oneSided) {
    if (p != NULL) {
        p->oneSided = oneSided;
    }
}

void polygon_copy(Polygon *to, Polygon *from) {
    if (to == NULL || from == NULL) {
        return;
    }

    to->oneSided = from->oneSided;
    to->nVertex = from->nVertex;

    free(to->vertex); // Free the existing vertices in 'to' if any
    to->vertex = malloc(sizeof(Point) * from->nVertex);
    if (to->vertex == NULL) {
        to->nVertex = 0; // Ensure consistency on allocation failure
        return;
    }
    for (int i = 0; i < from->nVertex; i++) {
        point_copy(&to->vertex[i], &from->vertex[i]); // Copy each vertex
    }
}

void polygon_print(Polygon *p, FILE *fp) {
    if (p == NULL || fp == NULL) {
        return;
    }

    fprintf(fp, "Polygon - One Sided: %s, Vertices: %d\n",
            (p->oneSided ? "Yes" : "No"), p->nVertex);
    for (int i = 0; i < p->nVertex; i++) {
        fprintf(fp, "Vertex %d: ", i + 1);
        point_print(&p->vertex[i], fp);
    }
}

void polygon_normalize(Polygon *p) {
    if (p == NULL) {
        return;
    }
    for (int i = 0; i < p->nVertex; i++) {
        point_normalize(&p->vertex[i]);
    }
}

void polygon_draw(Polygon *p, Image *src, Color c) {
    if (p == NULL || src == NULL || p->nVertex < 2) {
        return; // Not enough vertices to form a line
    }

    Line l;
    for (int i = 0; i < p->nVertex - 1; i++) {
        line_set(&l, p->vertex[i], p->vertex[i + 1]);
        line_draw(&l, src, c);
    }
    // Connect the last vertex back to the first to close the polygon
    line_set(&l, p->vertex[p->nVertex - 1], p->vertex[0]);
    line_draw(&l, src, c);
}