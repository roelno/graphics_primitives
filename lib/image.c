#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Image *image_create(int rows, int cols) {
  Image *img = (Image *)malloc(sizeof(Image));
  if (!img)
    return NULL;
  image_init(img);
  if (rows > 0 && cols > 0) {
    if (image_alloc(img, rows, cols) != 0) { // if allocation fails
      free(img);
      return NULL;
    }
  }
  return img;
}

void image_free(Image *src) {
  if (src) {
    image_dealloc(src);
    free(src);
  }
}

void image_init(Image *src) {
  if (src) {
    src->rows = 0;
    src->cols = 0;
    src->data = NULL;
    src->depth = NULL;
    src->alpha = NULL;
    src->maxval = 255.0f;
    src->filename = NULL;
  }
}

int image_alloc(Image *src, int rows, int cols) {
  if (!src || rows <= 0 || cols <= 0)
    return -1;

  image_dealloc(src); // in case the image already has data
  src->rows = rows;
  src->cols = cols;
  src->data = (FPixel **)malloc(rows * sizeof(FPixel *));
  if (!src->data)
    return -1;

  for (int i = 0; i < rows; i++) {
    src->data[i] = (FPixel *)malloc(cols * sizeof(FPixel));
    if (!src->data[i]) {
      for (int j = 0; j < i; j++)
        free(src->data[j]);
      free(src->data);
      src->data = NULL;
      return -1;
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      src->data[i][j].rgb[0] = 0.0f;
      src->data[i][j].rgb[1] = 0.0f;
      src->data[i][j].rgb[2] = 0.0f;
      src->data[i][j].a = 1.0f;
      src->data[i][j].z = 1.0f;
    }
  }
  return 0;
}

void image_dealloc(Image *src) {
  if (src && src->data) {
    for (int i = 0; i < src->rows; i++) {
      if (src->data[i])
        free(src->data[i]);
    }
    free(src->data);
    src->data = NULL;
  }
  src->rows = 0;
  src->cols = 0;
  src->depth = NULL;
  src->alpha = NULL;
  src->maxval = 255.0f;
  src->filename = NULL;
}

Image *image_read(char *filename) {
  char tag[40]; // PPM magic number
  Image *img;
  FILE *fp;
  int read, num[3], curchar;

  if (filename != NULL && strlen(filename)) {
    fp = fopen(filename, "r");
  } else {
    fp = stdin;
  }

  if (fp) {
    fscanf(fp, "%s\n", tag);

    // Read the "magic number" at the beginning of the ppm
    if (strncmp(tag, "P6", 40) != 0) {
      fprintf(stderr, "Not a PPM file!\n");
      exit(1);
    }

    // Read the rows, columns, and color depth, skipping comment lines
    read = 0;
    while (read < 3) {
      curchar = fgetc(fp);
      if ((char)curchar == '#') {
        while (fgetc(fp) != '\n')
          /* do nothing */;
      } else {
        ungetc(curchar, fp);
        fscanf(fp, "%d", &(num[read]));
        read++;
      }
    }
    while (fgetc(fp) != '\n')
      /* pass the last newline character */;

    int cols = num[0];
    int rows = num[1];
    int colors = num[2];

    img = image_create(rows, cols);
    if (!img) {
      if (fp != stdin)
        fclose(fp);
      return NULL;
    }

    img->maxval = (float)colors;

    // Read the data
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        unsigned char rgb[3];
        fread(rgb, sizeof(unsigned char), 3, fp);
        img->data[i][j].rgb[0] = rgb[0] / 255.0f;
        img->data[i][j].rgb[1] = rgb[1] / 255.0f;
        img->data[i][j].rgb[2] = rgb[2] / 255.0f;
        img->data[i][j].a = 1.0f; // Default alpha value
        img->data[i][j].z = 1.0f; // Default depth value
      }
    }

    if (fp != stdin)
      fclose(fp);
    return img;
  }

  return NULL;
}

int image_write(Image *src, char *filename) {
  FILE *fp;

  if (filename != NULL && strlen(filename)) {
    fp = fopen(filename, "w");
  } else {
    fp = stdout;
  }

  if (fp) {
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n%d\n", src->cols, src->rows, 255);

    for (int i = 0; i < src->rows; i++) {
      for (int j = 0; j < src->cols; j++) {
        unsigned char rgb[3];
        rgb[0] = (unsigned char)(src->data[i][j].rgb[0] * 255);
        rgb[1] = (unsigned char)(src->data[i][j].rgb[1] * 255);
        rgb[2] = (unsigned char)(src->data[i][j].rgb[2] * 255);
        fwrite(rgb, sizeof(unsigned char), 3, fp);
      }
    }

    if (fp != stdout)
      fclose(fp);
    return 0;
  }

  return -1;
}

FPixel image_getf(Image *src, int r, int c) { return src->data[r][c]; }

float image_getc(Image *src, int r, int c, int b) {
  return src->data[r][c].rgb[b];
}

float image_geta(Image *src, int r, int c) { return src->data[r][c].a; }

float image_getz(Image *src, int r, int c) { return src->data[r][c].z; }

void image_setf(Image *src, int r, int c, FPixel val) { src->data[r][c] = val; }

void image_setc(Image *src, int r, int c, int b, float val) {
  src->data[r][c].rgb[b] = val;
}

void image_seta(Image *src, int r, int c, float val) {
  src->data[r][c].a = val;
}

void image_setz(Image *src, int r, int c, float val) {
  src->data[r][c].z = val;
}

void image_reset(Image *src) {
  src->maxval = 255.0f;
  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->cols; j++) {
      src->data[i][j].rgb[0] = 0.0f;
      src->data[i][j].rgb[1] = 0.0f;
      src->data[i][j].rgb[2] = 0.0f;
      src->data[i][j].a = 1.0f;
      src->data[i][j].z = 1.0f;
    }
  }
}

void image_fill(Image *src, FPixel val) {
  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->cols; j++) {
      src->data[i][j] = val;
    }
  }
}

void image_fillrgb(Image *src, float r, float g, float b) {
  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->cols; j++) {
      src->data[i][j].rgb[0] = r;
      src->data[i][j].rgb[1] = g;
      src->data[i][j].rgb[2] = b;
    }
  }
}

void image_filla(Image *src, float a) {
  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->cols; j++) {
      src->data[i][j].a = a;
    }
  }
}

void image_fillz(Image *src, float z) {
  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->cols; j++) {
      src->data[i][j].z = z;
    }
  }
}
