#include "../include/polygon.h"
#include "../include/line.h"
#include <stdlib.h>

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
    free(p);         // Free the polygon structure itself
  }
}

void polygon_init(Polygon *p) {
  if (p != NULL) {
    p->oneSided = 0;
    p->nVertex = 0;
    free(p->vertex);  // Free existing vertices if any
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
    free(p->vertex);  // Free the vertex array
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

/**
 * @brief Calculates the Barycentric coordinates for a point with respect to a
 * triangle.
 * @param x X-coordinate of the point.
 * @param y Y-coordinate of the point.
 * @param x1, y1, x2, y2, x3, y3 Coordinates of the triangle's vertices.
 * @returns Array of three floats representing the Barycentric coordinates.
 */
static float *getBarycentric(float x, float y, float x1, float y1, float x2,
                             float y2, float x3, float y3) {
  float *bary = (float *)malloc(3 * sizeof(float));
  float detT = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
  bary[0] = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / detT;
  bary[1] = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / detT;
  bary[2] = 1.0f - bary[0] - bary[1];
  return bary;
}

void polygon_drawFillB(Polygon *p, Image *src, Color c) {
  // Find bounding box of the polygon
  float minX = p->vertex[0].val[0], maxX = minX;
  float minY = p->vertex[0].val[1], maxY = minY;
  for (int i = 1; i < p->nVertex; i++) {
    float vx = p->vertex[i].val[0], vy = p->vertex[i].val[1];
    if (vx < minX)
      minX = vx;
    if (vx > maxX)
      maxX = vx;
    if (vy < minY)
      minY = vy;
    if (vy > maxY)
      maxY = vy;
  }

  // Iterate over each pixel in the bounding box
  for (int y = (int)ceil(minY); y <= (int)floor(maxY); y++) {
    for (int x = (int)ceil(minX); x <= (int)floor(maxX); x++) {
      // Assume the polygon is a triangle for simplicity
      float *bary = getBarycentric(
          x, y, p->vertex[0].val[0], p->vertex[0].val[1], p->vertex[1].val[0],
          p->vertex[1].val[1], p->vertex[2].val[0], p->vertex[2].val[1]);

      // Check if point (x, y) is inside the triangle
      if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0) {
        if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
          src->data[y][x].rgb[0] = c.c[0];
          src->data[y][x].rgb[1] = c.c[1];
          src->data[y][x].rgb[2] = c.c[2];
          src->data[y][x].a = 1.0; // Assuming full opacity
        }
      }
      free(bary);
    }
  }
}