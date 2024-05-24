#include "line.h"
#include "image.h"
#include <math.h>
#include <stdlib.h>

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

void line_draw(Line *l, Image *src, Color c) {
  int x0 = (int)l->a.val[0];
  int y0 = (int)l->a.val[1];
  int x1 = (int)l->b.val[0];
  int y1 = (int)l->b.val[1];

  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx - dy;

  while (1) {
    if (x0 >= 0 && x0 < src->cols && y0 >= 0 && y0 < src->rows) {
      FPixel pixel;
      pixel.rgb[0] = c.c[0];
      pixel.rgb[1] = c.c[1];
      pixel.rgb[2] = c.c[2];
      pixel.a = 1.0; // Assuming full opacity for simplicity
      pixel.z = 0.0; // Assuming default depth for simplicity
      src->data[y0][x0] = pixel;
    }

    if (x0 == x1 && y0 == y1)
      break;
    int e2 = err * 2;
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
