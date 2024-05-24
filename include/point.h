#ifndef POINT_H
#define POINT_H

#include "image.h"
#include <stdio.h>

/**
 * @brief Structure representing a point in space.
 */
typedef struct {
  double val[4];
} Point;

void point_set2D(Point *p, double x, double y);
void point_set3D(Point *p, double x, double y, double z);
void point_set(Point *p, double x, double y, double z, double h);
void point_normalize(Point *p);
void point_copy(Point *to, Point *from);
void point_drawf(Point *p, Image *src, FPixel c);
void point_print(Point *p, FILE *fp);

#endif // POINT_H