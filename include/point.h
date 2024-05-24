#ifndef POINT_H
#define POINT_H

#include "color.h"
#include "image.h"
#include <stdio.h>

/**
 * @brief Structure representing a point in space.
 */
typedef struct {
  double val[4];
} Point;

/**
 * @brief Set the first two values of the vector to x and y.
 * Set the third value to 0.0 and the fourth value to 1.0.
 */
void point_set2D(Point *p, double x, double y);

/**
 * @brief Set the point's values to x, y, and z.
 * Set the homogeneous coordinate to 1.0.
 */
void point_set3D(Point *p, double x, double y, double z);

/**
 * @brief Set the four values of the vector to x, y, z, and h.
 */
void point_set(Point *p, double x, double y, double z, double h);

/**
 * @brief Normalize the x and y values of a point by its homogeneous coordinate.
 */
void point_normalize(Point *p);

/**
 * @brief Copy the point data structure.
 */
void point_copy(Point *to, Point *from);

/**
 * @brief Draw the point into src using color c.
 */
void point_draw(Point *p, Image *src, Color c);

/**
 * @brief Draw the point into src using FPixel c.
 */
void point_drawf(Point *p, Image *src, FPixel c);

/**
 * @brief Use fprintf to print the contents of the Point to the stream fp.
 */
void point_print(Point *p, FILE *fp);

#endif // POINT_H
