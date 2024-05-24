#include "point.h"


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

void point_draw(Point *p, Image *src, Color c) {
    point_normalize(p);

    int x = (int)p->val[0];
    int y = (int)p->val[1];

    if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
        image_setColor(src, x, y, c);
    } else {
        fprintf(stderr, "Point out of bounds: (%d, %d)\n", x, y);
    }
}

void point_drawf(Point *p, Image *src, FPixel c) {
    point_normalize(p);

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