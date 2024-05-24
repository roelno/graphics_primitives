#ifndef COLOR_IMAGE_H
#define COLOR_IMAGE_H

#include "image.h"


typedef struct {
    float c[3];
} Color;

/**
* @brief Copies the color values from one color to another
*/
void color_copy(Color *to, Color *from);

/**
* @brief Sets the color values to the given values
*/
void color_set(Color *to, float r, float g, float b);

/**
* @brief Sets the color of the pixel at the given row and column
*/
void image_setColor(Image *src, int r, int c, Color val);

/**
* @brief Gets the color of the pixel at the given row and column
*/
Color image_getColor(Image *src, int r, int c);

#endif // COLOR_IMAGE_H
