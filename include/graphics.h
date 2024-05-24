#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "point.h"
#include "line.h"
#include "image.h"
#include <stdio.h>

/**
 * @brief Structure representing a circle.
 */
typedef struct {
  double r;  ///< Radius of the circle
  Point c;   ///< Center of the circle
} Circle;

/**
 * @brief Sets the parameters of a circle.
 */
void circle_set(Circle *c, Point tc, double tr);

/**
 * @brief Draws the circle on the given image using the specified color.
 */
void circle_draw(Circle *c, Image *src, Color p);

/**
 * @brief Draws and fills the circle on the given image using the specified color.
 */
void circle_drawFill(Circle *c, Image *src, Color p);

/**
 * @brief Structure representing an ellipse.
 */
typedef struct {
  double ra;   ///< Semi-major axis radius
  double rb;   ///< Semi-minor axis radius
  Point c;     ///< Center of the ellipse
  double a;    ///< Angle of the major axis relative to the X-axis
} Ellipse;

/**
 * @brief Sets the parameters of an ellipse.
 */
void ellipse_set(Ellipse *e, Point tc, double ta, double tb);

/**
 * @brief Draws the ellipse on the given image using the specified color.
 */
void ellipse_draw(Ellipse *e, Image *src, Color p);

/**
 * @brief Draws and fills the ellipse on the given image using the specified color.
 */
void ellipse_drawFill(Ellipse *e, Image *src, Color p);

/**
 * @brief Structure representing a polyline.
 */
typedef struct {
  int zBuffer;    ///< Z-buffer flag
  int numVertex;  ///< Number of vertices
  Point *vertex;  ///< Array of vertices
} Polyline;

/**
 * @brief Creates and initializes an empty polyline.
 */
Polyline *polyline_create();

/**
 * @brief Creates and initializes a polyline with given vertices.
 */
Polyline *polyline_createp(int numV, Point *vlist);

/**
 * @brief Frees the memory allocated for the polyline.
 */
void polyline_free(Polyline *p);

/**
 * @brief Initializes a pre-existing polyline to an empty polyline.
 */
void polyline_init(Polyline *p);

/**
 * @brief Sets the vertices of a pre-existing polyline.
 */
void polyline_set(Polyline *p, int numV, Point *vlist);

/**
 * @brief Clears the vertices of a pre-existing polyline.
 */
void polyline_clear(Polyline *p);

/**
 * @brief Sets the z-buffer flag of a polyline.
 */
void polyline_zBuffer(Polyline *p, int flag);

/**
 * @brief Copies the vertices from one polyline to another.
 */
void polyline_copy(Polyline *to, Polyline *from);

/**
 * @brief Prints the polyline data to a file.
 */
void polyline_print(Polyline *p, FILE *fp);

/**
 * @brief Normalizes the vertices of a polyline.
 */
void polyline_normalize(Polyline *p);

/**
 * @brief Draws the polyline on the given image using the specified color.
 */
void polyline_draw(Polyline *p, Image *src, Color c);

#endif // GRAPHICS_H
