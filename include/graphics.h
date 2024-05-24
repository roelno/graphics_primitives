#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "image.h" // Include the image.h header file
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

/**
 * @brief Structure representing a line.
 */
typedef struct {
  int zBuffer; // Whether to use the z-buffer, should default to true (1)
  Point a;
  Point b;
} Line;

void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);
void line_normalize(Line *l);
void line_copy(Line *to, Line *from);
void line_draw(Line *l, Image *src, FPixel c);

/**
 * @brief Structure representing a circle.
 */
typedef struct {
  double r;
  Point c;
} Circle;

void circle_set(Circle *c, Point tc, double tr);
void circle_draw(Circle *c, Image *src, FPixel p);
void circle_drawFill(Circle *c, Image *src, FPixel p);

/**
 * @brief Structure representing an ellipse.
 */
typedef struct {
  double ra;
  double rb;
  Point c;
  double a; // Angle of major axis relative to the X-axis
} Ellipse;

void ellipse_set(Ellipse *e, Point tc, double ta, double tb);
void ellipse_draw(Ellipse *e, Image *src, FPixel p);
void ellipse_drawFill(Ellipse *e, Image *src, FPixel p);

/**
 * @brief Structure representing a polyline.
 */
typedef struct {
  int zBuffer;
  int numVertex;
  Point *vertex;
} Polyline;

Polyline *polyline_create(void);
Polyline *polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);
void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);
void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_normalize(Polyline *p);
void polyline_draw(Polyline *p, Image *src, FPixel c);

#endif // GRAPHICS_H
