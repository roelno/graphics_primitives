#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "point.h"
#include "image.h"
#include <stdio.h>


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
