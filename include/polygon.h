#ifndef POLYGON_H
#define POLYGON_H
#include "point.h"

typedef struct {
  int oneSided;  // whether to consider the polygon one-sided (1) or two-sided
                 // (0) for shading
  int nVertex;   // number of vertices
  Point *vertex; // array of vertices
} Polygon;

/// The functions polygon create and polygon free manage both the Polygon data
/// structure and the memory required for the vertex list.

/**
 * @brief returns an allocated Polygon pointer initialized so that numVertex is
 * 0 and vertex is NULL.
 */
void *polygon_create();

/**
 * @brief returns an allocated Polygon pointer with the vertex list initialized
 * to a copy of the points in vlist.
 */
void *polygon_createp(int numV, Point *vlist);

/**
 * @brief frees the internal data for a Polygon and the Polygon pointer.
 */
void polygon_free(Polygon *p);

/// The functions polygon init, polygon set, and polygon clear work on a
/// pre-existing Polygon data structure and manage only the memory required for
/// the vertex list.

/**
 * @brief initializes the existing Polygon to an empty Polygon.
 */
void polygon_init(Polygon *p);

/**
 * @brief initializes the vertex array to the points in vlist.
 */
void polygon_set(Polygon *p, int numV, Point *vlist);

/**
 * @brief frees the internal data and resets the fields.
 */
void polygon_clear(Polygon *p);

/**
 * @brief sets the oneSided field to the value.
 */
void polygon_setSided(Polygon *p, int oneSided);

// /**
//  * @brief initializes the color array to the colors in clist.
//  */
// void polygon_setColors(Polygon *p, int numV, Color *clist);

// /**
//  * @brief initializes the normal array to the vectors in nlist.
//  */
// void polygon_setNormals(Polygon *p, int numV, Vector *nlist);

// /**
//  * @brief initializes the vertex list to the points in vlist, the colors to the
// colors in clist, the normals to the vectors in nlist, and the zBuffer and
// oneSided flags to their respectively values.
//  */
// void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist,
//                     Vector *nlist, int zBuffer, int oneSided);

// /**
//  * @brief sets the z-buffer flag to the given value.
//  */
// void polygon_zBuffer(Polygon *p, int flag);

/**
 * @brief De-allocates/allocates space and copies the vertex and color data from
 * one polygon to the other.
 */
void polygon_copy(Polygon *to, Polygon *from);

/**
 * @brief prints polygon data to the stream designated by the FILE pointer.
 */
void polygon_print(Polygon *p, FILE *fp);

/**
 * @brief normalize the x and y values of each vertex by the homogeneous coord.
 */
void polygon_normalize(Polygon *p);

/**
 * @brief draw the outline of the polygon using color c.
 */
void polygon_draw(Polygon *p, Image *src, Color c);

/**
 * @brief draw the filled polygon using color c with the scanline z-buffer rendering algorithm.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c);

/**
 * @brief draw the filled polygon using color c with the Barycentric coordinates algorithm.
 */
void polygon_drawFillB(Polygon *p, Image *src, Color c);

#endif // POLYGON_H