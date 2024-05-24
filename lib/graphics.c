#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"
#include "point.h"
#include "line.h"


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

Polyline *polyline_create(void) {
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