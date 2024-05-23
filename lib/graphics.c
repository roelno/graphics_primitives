#include "image.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  double val[4];
} Point;

typedef struct {
  int zBuffer; // Whether to use the z-buffer, should default to true (1)
  Point a;
  Point b;
} Line;

typedef struct {
  double r;
  Point c;
} Circle;

typedef struct {
  double ra;
  double rb;
  Point c;
  double a; // Angle of major axis relative to the X-axis
} Ellipse;

typedef struct {
  int zBuffer;
  int numVertex;
  Point *vertex;
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
      if (x0 + i >= 0 && x0 + i < src->cols && y0 + y >= 0 &&
          y0 + y < src->rows)
        src->data[y0 + y][x0 + i] = p;
      if (x0 + i >= 0 && x0 + i < src->cols && y0 - y >= 0 &&
          y0 - y < src->rows)
        src->data[y0 - y][x0 + i] = p;
    }
    for (int i = -y; i <= y; i++) {
      if (x0 + i >= 0 && x0 + i < src->cols && y0 + x >= 0 &&
          y0 + x < src->rows)
        src->data[y0 + x][x0 + i] = p;
      if (x0 + i >= 0 && x0 + i < src->cols && y0 - x >= 0 &&
          y0 - x < src->rows)
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

void ellipse_set(Ellipse *e, Point tc, double ta, double tb) {
  point_copy(&(e->c), &tc);
  e->ra = ta;
  e->rb = tb;
  e->a = 0.0;
}

void ellipse_draw(Ellipse *e, Image *src, FPixel p) {
  // use midpoint ellipse algorithm
  int xc = (int)e->c.val[0];
  int yc = (int)e->c.val[1];
  int a = (int)e->ra;
  int b = (int)e->rb;

  int x = 0;
  int y = b;
  double a2 = a * a;
  double b2 = b * b;
  double crit1 = -(a2 / 4 + a % 2 + b2);
  double crit2 = -(b2 / 4 + b % 2 + a2);
  double crit3 = -(b2 / 4 + b % 2);
  double t = -a2 * y;
  double dxt = 2 * b2 * x;
  double dyt = -2 * a2 * y;
  double d2xt = 2 * b2;
  double d2yt = 2 * a2;

  while (y >= 0 && x <= a) {
    if (xc + x >= 0 && xc + x < src->cols && yc + y >= 0 && yc + y < src->rows)
      src->data[yc + y][xc + x] = p;
    if (xc - x >= 0 && xc - x < src->cols && yc + y >= 0 && yc + y < src->rows)
      src->data[yc + y][xc - x] = p;
    if (xc + x >= 0 && xc + x < src->cols && yc - y >= 0 && yc - y < src->rows)
      src->data[yc - y][xc + x] = p;
    if (xc - x >= 0 && xc - x < src->cols && yc - y >= 0 && yc - y < src->rows)
      src->data[yc - y][xc - x] = p;

    if (t + b2 * x <= crit1 || t + a2 * y <= crit3) {
      x++;
      dxt += d2xt;
      t += dxt;
    } else if (t - a2 * y > crit2) {
      y--;
      dyt += d2yt;
      t += dyt;
    } else {
      x++;
      dxt += d2xt;
      t += dxt;
      y--;
      dyt += d2yt;
      t += dyt;
    }
  }
}

void ellipse_drawFill(Ellipse *e, Image *src, FPixel p) {
  // use midpoint ellipse algorithm
  int xc = (int)e->c.val[0];
  int yc = (int)e->c.val[1];
  int a = (int)e->ra;
  int b = (int)e->rb;

  int x = 0;
  int y = b;
  double a2 = a * a;
  double b2 = b * b;
  double crit1 = -(a2 / 4 + a % 2 + b2);
  double crit2 = -(b2 / 4 + b % 2 + a2);
  double crit3 = -(b2 / 4 + b % 2);
  double t = -a2 * y;
  double dxt = 2 * b2 * x;
  double dyt = -2 * a2 * y;
  double d2xt = 2 * b2;
  double d2yt = 2 * a2;

  while (y >= 0 && x <= a) {
    for (int i = xc - x; i <= xc + x; i++) {
      if (i >= 0 && i < src->cols && yc + y >= 0 && yc + y < src->rows)
        src->data[yc + y][i] = p;
      if (i >= 0 && i < src->cols && yc - y >= 0 && yc - y < src->rows)
        src->data[yc - y][i] = p;
    }

    if (t + b2 * x <= crit1 || t + a2 * y <= crit3) {
      x++;
      dxt += d2xt;
      t += dxt;
    } else if (t - a2 * y > crit2) {
      y--;
      dyt += d2yt;
      t += dyt;
    } else {
      x++;
      dxt += d2xt;
      t += dxt;
      y--;
      dyt += d2yt;
      t += dyt;
    }
  }
}

Polyline *polyline_create() {
  Polyline *p = (Polyline *)malloc(sizeof(Polyline));
  if (!p)
    return NULL;
  p->zBuffer = 1;
  p->numVertex = 0;
  p->vertex = NULL;
  return p;
}

Polyline *polyline_createp(int numV, Point *vlist) {
  Polyline *p = (Polyline *)malloc(sizeof(Polyline));
  if (!p)
    return NULL;
  p->zBuffer = 1;
  p->numVertex = numV;
  p->vertex = (Point *)malloc(numV * sizeof(Point));
  if (!p->vertex) {
    free(p);
    return NULL;
  }
  memcpy(p->vertex, vlist, numV * sizeof(Point));
  return p;
}

void polyline_free(Polyline *p) {
  if (p) {
    if (p->vertex) {
      free(p->vertex);
    }
    free(p);
  }
}

void polyline_init(Polyline *p) {
  if (p) {
    p->zBuffer = 1;
    p->numVertex = 0;
    p->vertex = NULL;
  }
}

void polyline_set(Polyline *p, int numV, Point *vlist) {
  if (p) {
    if (p->vertex) {
      free(p->vertex);
    }
    p->numVertex = numV;
    p->vertex = (Point *)malloc(numV * sizeof(Point));
    if (!p->vertex) {
      p->numVertex = 0;
      return;
    }
    memcpy(p->vertex, vlist, numV * sizeof(Point));
  }
}

void polyline_clear(Polyline *p) {
  if (p) {
    if (p->vertex) {
      free(p->vertex);
      p->vertex = NULL;
    }
    p->numVertex = 0;
  }
}

void polyline_zBuffer(Polyline *p, int flag) {
  if (p) {
    p->zBuffer = flag;
  }
}

void polyline_copy(Polyline *to, Polyline *from) {
  if (to && from) {
    if (to->vertex) {
      free(to->vertex);
    }
    to->numVertex = from->numVertex;
    to->vertex = (Point *)malloc(from->numVertex * sizeof(Point));
    if (!to->vertex) {
      to->numVertex = 0;
      return;
    }
    memcpy(to->vertex, from->vertex, from->numVertex * sizeof(Point));
    to->zBuffer = from->zBuffer;
  }
}

void polyline_print(Polyline *p, FILE *fp) {
  if (p && fp) {
    fprintf(fp, "Polyline: numVertex = %d, zBuffer = %d\n", p->numVertex,
            p->zBuffer);
    for (int i = 0; i < p->numVertex; i++) {
      fprintf(fp, "Vertex %d: (%f, %f, %f, %f)\n", i, p->vertex[i].val[0],
              p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
    }
  }
}

void polyline_normalize(Polyline *p) {
  if (p && p->vertex) {
    for (int i = 0; i < p->numVertex; i++) {
      point_normalize(&(p->vertex[i]));
    }
  }
}

void polyline_draw(Polyline *p, Image *src, FPixel c) {
  if (p && p->vertex && p->numVertex > 1) {
    for (int i = 0; i < p->numVertex - 1; i++) {
      Line l;
      line_set(&l, p->vertex[i], p->vertex[i + 1]);
      line_zBuffer(&l, p->zBuffer);
      line_draw(&l, src, c);
    }
  }
}