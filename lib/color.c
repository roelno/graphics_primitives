#include "color.h"

void color_copy(Color *to, Color *from) {
    to->c[0] = from->c[0];
    to->c[1] = from->c[1];
    to->c[2] = from->c[2];
}

void color_set(Color *to, float r, float g, float b) {
    to->c[0] = r;
    to->c[1] = g;
    to->c[2] = b;
}

void image_setColor(Image *src, int r, int c, Color val) {
    if (r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        // Handle out of bounds error
        return;
    }
    src->data[r][c].rgb[0] = val.c[0];
    src->data[r][c].rgb[1] = val.c[1];
    src->data[r][c].rgb[2] = val.c[2];
}

Color image_getColor(Image *src, int r, int c) {
    Color result = { { 0.0f, 0.0f, 0.0f } };  // Default to black
    if (r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        // Handle out of bounds error
        return result;
    }
    result.c[0] = src->data[r][c].rgb[0];
    result.c[1] = src->data[r][c].rgb[1];
    result.c[2] = src->data[r][c].rgb[2];
    return result;
}