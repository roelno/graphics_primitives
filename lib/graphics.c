#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to draw a pixel in the image
static void draw_pixel(Image *src, int x, int y, Color p) {
  if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
    image_setColor(src, y, x, p);
  }
}

void circle_set(Circle *c, Point tc, double tr) {
  point_copy(&(c->c), &tc);
  c->r = tr;
}

void circle_draw(Circle *c, Image *src, Color p) {
  int x0 = (int)c->c.val[0];
  int y0 = (int)c->c.val[1];
  int r = (int)c->r;
  int x = r;
  int y = 0;
  int err = 1 - x;

  while (x >= y) {
    draw_pixel(src, x0 + x, y0 + y, p);
    draw_pixel(src, x0 + y, y0 + x, p);
    draw_pixel(src, x0 - y, y0 + x, p);
    draw_pixel(src, x0 - x, y0 + y, p);
    draw_pixel(src, x0 - x, y0 - y, p);
    draw_pixel(src, x0 - y, y0 - x, p);
    draw_pixel(src, x0 + y, y0 - x, p);
    draw_pixel(src, x0 + x, y0 - y, p);

    y += 1;
    if (err <= 0) {
      err += 2 * y + 1;
    } else {
      x -= 1;
      err += 2 * (y - x) + 1;
    }
  }
}

void circle_drawFill(Circle *c, Image *src, Color p) {
  int x0 = (int)c->c.val[0];
  int y0 = (int)c->c.val[1];
  int r = (int)c->r;

  for (int y = -r; y <= r; y++) {
    for (int x = -r; x <= r; x++) {
      if (x * x + y * y <= r * r) {
        draw_pixel(src, x0 + x, y0 + y, p);
      }
    }
  }
}

void ellipse_set(Ellipse *e, Point tc, double ta, double tb) {
  point_copy(&(e->c), &tc);
  e->ra = ta;
  e->rb = tb;
  e->a = 0.0;
}

void ellipse_draw(Ellipse *e, Image *src, Color p) {
  // use midpoint ellipse algorithm
  int x0 = (int)e->c.val[0];
  int y0 = (int)e->c.val[1];
  int ra = (int)e->ra;
  int rb = (int)e->rb;

  int x = ra;
  int y = 0;
  int ra2 = ra * ra;
  int rb2 = rb * rb;
  int err = ra2 - ra * rb2 + rb2 / 4;

  // Region 1
  while (ra2 * y <= rb2 * x) {
    draw_pixel(src, x0 + x, y0 + y, p);
    draw_pixel(src, x0 - x, y0 + y, p);
    draw_pixel(src, x0 - x, y0 - y, p);
    draw_pixel(src, x0 + x, y0 - y, p);

    y++;
    if (err <= 0) {
      err += 2 * y * ra2 + ra2;
    } else {
      x--;
      err += 2 * y * ra2 - 2 * x * rb2 + ra2;
    }
  }

  // Region 2
  x = 0;
  y = rb;
  err = rb2 - rb * ra2 + ra2 / 4;

  while (rb2 * x <= ra2 * y) {
    draw_pixel(src, x0 + x, y0 + y, p);
    draw_pixel(src, x0 - x, y0 + y, p);
    draw_pixel(src, x0 - x, y0 - y, p);
    draw_pixel(src, x0 + x, y0 - y, p);

    x++;
    if (err <= 0) {
      err += 2 * x * rb2 + rb2;
    } else {
      y--;
      err += 2 * x * rb2 - 2 * y * ra2 + rb2;
    }
  }
}


void ellipse_drawFill(Ellipse *e, Image *src, Color p) {
  int x0 = (int)e->c.val[0];
  int y0 = (int)e->c.val[1];
  int ra = (int)e->ra;
  int rb = (int)e->rb;

  for (int y = -rb; y <= rb; y++) {
    for (int x = -ra; x <= ra; x++) {
      if ((x * x) * (rb * rb) + (y * y) * (ra * ra) <= (ra * ra) * (rb * rb)) {
        draw_pixel(src, x0 + x, y0 + y, p);
      }
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
  if (p != NULL) {
    if (p->vertex != NULL) {
      free(p->vertex);
    }
    p->numVertex = numV;
    p->vertex = (Point *)malloc(numV * sizeof(Point));
    if (p->vertex == NULL) {
      p->numVertex = 0;
      return;
    }
    memcpy(p->vertex, vlist, numV * sizeof(Point));
  }
}

void polyline_clear(Polyline *p) {
  if (p != NULL && p->vertex != NULL) {
    free(p->vertex);
    p->vertex = NULL;
    p->numVertex = 0;
  }
}

void polyline_zBuffer(Polyline *p, int flag) {
  if (p) {
    p->zBuffer = flag;
  }
}

void polyline_copy(Polyline *to, Polyline *from) {
  if (to != NULL && from != NULL) {
    polyline_set(to, from->numVertex, from->vertex);
    to->zBuffer = from->zBuffer;
  }
}

void polyline_print(Polyline *p, FILE *fp) {
  if (p != NULL && fp != NULL) {
    fprintf(fp, "Polyline: zBuffer=%d, numVertex=%d\n", p->zBuffer,
            p->numVertex);
    for (int i = 0; i < p->numVertex; i++) {
      point_print(&p->vertex[i], fp);
    }
  }
}

void polyline_normalize(Polyline *p) {
  if (p != NULL && p->vertex != NULL) {
    for (int i = 0; i < p->numVertex; i++) {
      point_normalize(&p->vertex[i]);
    }
  }
}

void polyline_draw(Polyline *p, Image *src, Color c) {
  if (p != NULL && src != NULL && p->vertex != NULL && p->numVertex > 1) {
    for (int i = 0; i < p->numVertex - 1; i++) {
      Line l;
      line_set(&l, p->vertex[i], p->vertex[i + 1]);
      line_zBuffer(&l, p->zBuffer);
      line_draw(&l, src, c);
    }
  }
}