#include "image.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  double val[4]; // Four element vector of doubles
} Point;

typedef struct {
  int zBuffer; // Whether to use the z-buffer, should default to true (1)
  Point a;     // Starting point
  Point b;     // Ending point
} Line;

typedef struct {
  double r; // Radius
  Point c;  // Center
} Circle;

typedef struct {
  double ra; // Major axis radius
  double rb; // Minor axis radius
  Point c;   // Center
  double a;  // Angle of major axis relative to the X-axis (optional)
} Ellipse;

typedef struct {
  int zBuffer;   // Whether to use the z-buffer; should default to true (1)
  int numVertex; // Number of vertices
  Point *vertex; // Vertex information
} Polyline;

void point_set2D(Point *p, double x, double y) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = 0.0;
  p->val[3] = 1.0;
}

void point_set3D(Point *p, double x, double y, double z) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = 1.0;
}

void point_set(Point *p, double x, double y, double z, double h) {
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = h;
}

void point_normalize(Point *p) {
  if (p->val[3] != 0) {
    p->val[0] /= p->val[3];
    p->val[1] /= p->val[3];
  }
}

void point_copy(Point *to, Point *from) {
  for (int i = 0; i < 4; i++) {
    to->val[i] = from->val[i];
  }
}

void point_drawf(Point *p, Image *src, FPixel c) {
  int x = (int)p->val[0];
  int y = (int)p->val[1];

  if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
    src->data[y][x] = c;
  } else {
    fprintf(stderr, "Point out of bounds: (%d, %d)\n", x, y);
  }
}

void point_print(Point *p, FILE *fp) {
  fprintf(fp, "Point: (%f, %f, %f, %f)\n", p->val[0], p->val[1], p->val[2],
          p->val[3]);
}

void line_set2D(Line *l, double x0, double y0, double x1, double y1) {
  point_set2D(&(l->a), x0, y0);
  point_set2D(&(l->b), x1, y1);
  l->zBuffer = 1;
}

void line_set(Line *l, Point ta, Point tb) {
  point_copy(&(l->a), &ta);
  point_copy(&(l->b), &tb);
  l->zBuffer = 1;
}

void line_zBuffer(Line *l, int flag) { l->zBuffer = flag; }

void line_normalize(Line *l) {
  point_normalize(&(l->a));
  point_normalize(&(l->b));
}

void line_copy(Line *to, Line *from) {
  point_copy(&(to->a), &(from->a));
  point_copy(&(to->b), &(from->b));
  to->zBuffer = from->zBuffer;
}

void line_draw(Line *l, Image *src, FPixel c) {
  int x0 = (int)l->a.val[0];
  int y0 = (int)l->a.val[1];
  int x1 = (int)l->b.val[0];
  int y1 = (int)l->b.val[1];

  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    // Set pixel at (x0, y0) if within bounds
    if (x0 >= 0 && x0 < src->cols && y0 >= 0 && y0 < src->rows) {
      // Check z-buffer condition if applicable
      if (!l->zBuffer || c.z < image_getz(src, y0, x0)) {
        src->data[y0][x0] = c;
        if (l->zBuffer) {
          image_setz(src, y0, x0, c.z);
        }
      }
    }

    // Check if reached the end point
    if (x0 == x1 && y0 == y1)
      break;

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void circle_set(Circle *c, Point tc, double tr) {
    point_copy(&(c->c), &tc);
    c->r = tr;
}

void circle_draw(Circle *c, Image *src, FPixel p) {
    // using midpoint circle algorithm
    int x0 = (int)c->c.val[0];
    int y0 = (int)c->c.val[1];
    int radius = (int)c->r;
    
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;
    
    while (x >= y) {
        if (x0 + x >= 0 && x0 + x < src->cols && y0 + y >= 0 && y0 + y < src->rows)
            src->data[y0 + y][x0 + x] = p;
        if (x0 - x >= 0 && x0 - x < src->cols && y0 + y >= 0 && y0 + y < src->rows)
            src->data[y0 + y][x0 - x] = p;
        if (x0 + x >= 0 && x0 + x < src->cols && y0 - y >= 0 && y0 - y < src->rows)
            src->data[y0 - y][x0 + x] = p;
        if (x0 - x >= 0 && x0 - x < src->cols && y0 - y >= 0 && y0 - y < src->rows)
            src->data[y0 - y][x0 - x] = p;
        if (x0 + y >= 0 && x0 + y < src->cols && y0 + x >= 0 && y0 + x < src->rows)
            src->data[y0 + x][x0 + y] = p;
        if (x0 - y >= 0 && x0 - y < src->cols && y0 + x >= 0 && y0 + x < src->rows)
            src->data[y0 + x][x0 - y] = p;
        if (x0 + y >= 0 && x0 + y < src->cols && y0 - x >= 0 && y0 - x < src->rows)
            src->data[y0 - x][x0 + y] = p;
        if (x0 - y >= 0 && x0 - y < src->cols && y0 - x >= 0 && y0 - x < src->rows)
            src->data[y0 - x][x0 - y] = p;

        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}

void circle_drawFill(Circle *c, Image *src, FPixel p) {
    // using midpoint circle algorithm
    int x0 = (int)c->c.val[0];
    int y0 = (int)c->c.val[1];
    int radius = (int)c->r;
    
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;
    
    while (x >= y) {
        for (int i = -x; i <= x; i++) {
            if (x0 + i >= 0 && x0 + i < src->cols && y0 + y >= 0 && y0 + y < src->rows)
                src->data[y0 + y][x0 + i] = p;
            if (x0 + i >= 0 && x0 + i < src->cols && y0 - y >= 0 && y0 - y < src->rows)
                src->data[y0 - y][x0 + i] = p;
        }
        for (int i = -y; i <= y; i++) {
            if (x0 + i >= 0 && x0 + i < src->cols && y0 + x >= 0 && y0 + x < src->rows)
                src->data[y0 + x][x0 + i] = p;
            if (x0 + i >= 0 && x0 + i < src->cols && y0 - x >= 0 && y0 - x < src->rows)
                src->data[y0 - x][x0 + i] = p;
        }

        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}