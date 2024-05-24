#ifndef LINE_H
#define LINE_H

#include "point.h"
#include "color.h"
#include "image.h"

/**
 * @brief Structure representing a line.
 */
typedef struct {
  int zBuffer; // Whether to use the z-buffer, should default to true (1)
  Point a;
  Point b;
} Line;

/**
 * @brief Initialize a 2D line.
 */
void line_set2D(Line *l, double x0, double y0, double x1, double y1);

/**
 * @brief Initialize a line to ta and tb.
 */
void line_set(Line *l, Point ta, Point tb);

/**
 * @brief Set the z-buffer flag to the given value.
 */
void line_zBuffer(Line *l, int flag);

/**
 * @brief Normalize the x and y values of the endpoints by their homogeneous coordinate.
 */
void line_normalize(Line *l);

/**
 * @brief Copy the line data structure.
 */
void line_copy(Line *to, Line *from);

/**
 * @brief Draw the line into src using color c and the z-buffer, if appropriate.
 */
void line_draw(Line *l, Image *src, Color c);

#endif // LINE_H
