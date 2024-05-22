#ifndef IMAGE_H
#define IMAGE_H

/**
 * @file image.h
 * @brief Header file for image manipulation functions and structures.
 *
 * This file contains the definitions of the FPixel and Image structures,
 * along with function declarations for creating, manipulating, and
 * processing images.
 */

/**
 * @brief Structure representing a pixel with RGB, alpha, and depth values.
 */
typedef struct {
  float rgb[3];
  float a;
  float z;
} FPixel;

/**
 * @brief Structure representing an image with various properties.
 */
typedef struct {
  FPixel **data; // Pointer to the space for storing pixels
  int rows;
  int cols;
  float *depth;
  float *alpha;
  float maxval;
  char *filename;
} Image;

// Constructors and destructors

/**
 * @brief Allocates and initializes an Image structure.
 *
 * @param rows Number of rows in the image.
 * @param cols Number of columns in the image.
 * @return Pointer to the allocated Image structure, or NULL if allocation
 * fails.
 */
Image *image_create(int rows, int cols);

/**
 * @brief Deallocates the image data and frees the Image structure.
 *
 * @param src Pointer to the Image structure to be freed.
 */
void image_free(Image *src);

/**
 * @brief Initializes an uninitialized Image structure.
 *
 * @param src Pointer to the Image structure to be initialized.
 */
void image_init(Image *src);

/**
 * @brief Allocates space for the image data.
 *
 * @param src Pointer to the Image structure.
 * @param rows Number of rows in the image.
 * @param cols Number of columns in the image.
 * @return 0 if successful, non-zero otherwise.
 */
int image_alloc(Image *src, int rows, int cols);

/**
 * @brief Deallocates the image data and resets the Image structure fields.
 *
 * @param src Pointer to the Image structure.
 */
void image_dealloc(Image *src);

// I/O functions

/**
 * @brief Reads a PPM image from the given filename.
 *
 * @param filename Name of the file to be read.
 * @return Pointer to the Image structure, or NULL if the operation fails.
 */
Image *image_read(char *filename);

/**
 * @brief Writes the image to a PPM file with the given filename.
 *
 * @param src Pointer to the Image structure.
 * @param filename Name of the file to be written.
 * @return 0 if successful, non-zero otherwise.
 */
int image_write(Image *src, char *filename);

// Access functions

/**
 * @brief Returns the FPixel at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @return FPixel at the specified location.
 */
FPixel image_getf(Image *src, int r, int c);

/**
 * @brief Returns the value of a specific color band at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @param b Index of the color band (0 for red, 1 for green, 2 for blue).
 * @return Value of the specified color band.
 */
float image_getc(Image *src, int r, int c, int b);

/**
 * @brief Returns the alpha value at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @return Alpha value at the specified location.
 */
float image_geta(Image *src, int r, int c);

/**
 * @brief Returns the depth value at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @return Depth value at the specified location.
 */
float image_getz(Image *src, int r, int c);

/**
 * @brief Sets the values of a pixel at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @param val FPixel value to be set.
 */
void image_setf(Image *src, int r, int c, FPixel val);

/**
 * @brief Sets the value of a specific color band at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @param b Index of the color band (0 for red, 1 for green, 2 for blue).
 * @param val Value to be set.
 */
void image_setc(Image *src, int r, int c, int b, float val);

/**
 * @brief Sets the alpha value at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @param val Alpha value to be set.
 */
void image_seta(Image *src, int r, int c, float val);

/**
 * @brief Sets the depth value at the specified location.
 *
 * @param src Pointer to the Image structure.
 * @param r Row index of the pixel.
 * @param c Column index of the pixel.
 * @param val Depth value to be set.
 */
void image_setz(Image *src, int r, int c, float val);

// Utility functions

/**
 * @brief Resets every pixel to a default value (black with full alpha and
 * depth).
 *
 * @param src Pointer to the Image structure.
 */
void image_reset(Image *src);

/**
 * @brief Sets every pixel to the given FPixel value.
 * 
 * @param src Pointer to the Image structure.
 * @param val FPixel value to be set for every pixel.
 */
void image_fill(Image *src, FPixel val);

/**
 * @brief Sets the RGB values of each pixel to the given color.
 * 
 * @param src Pointer to the Image structure.
 * @param r Red value to be set.
 * @param g Green value to be set.
 * @param b Blue value to be set.
 */
void image_fillrgb(Image *src, float r, float g, float b);

/**
 * @brief Sets the alpha value of each pixel to the given value.
 * 
 * @param src Pointer to the Image structure.
 * @param a Alpha value to be set.
 */
void image_filla(Image *src, float a);

/**
 * @brief Sets the depth value of each pixel to the given value.
 * 
 * @param src Pointer to the Image structure.
 * @param z Depth value to be set.
 */
void image_fillz(Image *src, float z);

#endif