/*
  Bruce A. Maxwell
  Fall 2014

        Skeleton scanline fill algorithm
*/

#include "../include/list.h"
#include "../include/polygon.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************
Scanline Fill Algorithm
********************/

// define the struct here, because it is local to only this file
typedef struct tEdge {
  float x0, y0;                /* start point for the edge */
  float x1, y1;                /* end point for the edge */
  int yStart, yEnd;            /* start row and end row */
  float xIntersect, dxPerScan; /* where the edge intersects the current scanline
                                  and how it changes */
                               /* we'll add more here later */
  struct tEdge *next;
} Edge;

/*
        This is a comparison function that returns a value < 0 if a < b, a
        value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
        Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart(const void *a, const void *b) {
  Edge *ea = (Edge *)a;
  Edge *eb = (Edge *)b;

  return (ea->yStart - eb->yStart);
}

/*
        This is a comparison function that returns a value < 0 if a < b, a
        value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
        Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect(const void *a, const void *b) {
  Edge *ea = (Edge *)a;
  Edge *eb = (Edge *)b;

  if (ea->xIntersect < eb->xIntersect)
    return (-1);
  else if (ea->xIntersect > eb->xIntersect)
    return (1);

  return (0);
}

/*
        Allocates, creates, fills out, and returns an Edge structure given
        the inputs.

        Current inputs are just the start and end location in image space.
        Eventually, the points will be 3D and we'll add color and texture
        coordinates.
 */
static Edge *makeEdgeRec(Point start, Point end, Image *src) {
  // Allocate memory for the edge structure
  Edge *edge = malloc(sizeof(Edge));
  if (!edge) {
    printf("Unable to allocate memory for an edge.\n");
    return NULL;
  }

  // Initialize the edge structure with start and end points
  edge->x0 = start.val[0];
  edge->y0 = start.val[1];
  edge->x1 = end.val[0];
  edge->y1 = end.val[1];

  // Clip the edge if it starts below the image or ends above the image
  if (edge->y1 < 0 || edge->y0 >= src->rows) {
    free(edge);
    return NULL;
  }

  // Adjust y0 to the nearest integer and assign it to yStart
  edge->yStart = (int)(edge->y0 + 0.5);

  // Adjust y1 to the nearest integer and subtract 1 and assign it to yEnd
  edge->yEnd = (int)(edge->y1 + 0.5) - 1;

  // Clip yEnd to the number of rows - 1
  if (edge->yEnd >= src->rows) {
    edge->yEnd = src->rows - 1;
  }

  // Check if the edge should be included based on its position relative to the
  // image
  if (edge->yStart >= src->rows || edge->yEnd < 0) {
    free(edge);
    return NULL;
  }

  // Calculate the slope of the edge
  float dscan = end.val[1] - start.val[1];
  edge->dxPerScan = (end.val[0] - start.val[0]) / dscan;

  // Calculate the initial intersection with the scanline
  edge->xIntersect =
      edge->x0 + (0.5 - (edge->y0 - edge->yStart)) * edge->dxPerScan;

  // Adjust the edge if it starts above the image
  if (edge->yStart < 0) {
    edge->xIntersect += (-edge->yStart) * edge->dxPerScan;
    edge->y0 = 0;
    edge->yStart = 0;
  }

  // Check for steep slopes that might cause xIntersect to go beyond the edge's
  // x1
  if ((edge->dxPerScan > 0 && edge->xIntersect > edge->x1) ||
      (edge->dxPerScan < 0 && edge->xIntersect < edge->x0)) {
    free(edge);
    return NULL;
  }

  // Return the initialized edge structure
  return edge;
}

/*
        Returns a list of all the edges in the polygon in sorted order by
        smallest row.
*/
static LinkedList *setupEdgeList(Polygon *p, Image *src) {
  LinkedList *edges = NULL;
  Point v1, v2;
  int i;

  // create a linked list
  edges = ll_new();

  // walk around the polygon, starting with the last point
  v1 = p->vertex[p->nVertex - 1];

  for (i = 0; i < p->nVertex; i++) {

    // the current point (i) is the end of the segment
    v2 = p->vertex[i];

    // if it is not a horizontal line
    if ((int)(v1.val[1] + 0.5) != (int)(v2.val[1] + 0.5)) {
      Edge *edge;

      // if the first coordinate is smaller (top edge)
      if (v1.val[1] < v2.val[1])
        edge = makeEdgeRec(v1, v2, src);
      else
        edge = makeEdgeRec(v2, v1, src);

      // insert the edge into the list of edges if it's not null
      if (edge)
        ll_insert(edges, edge, compYStart);
    }
    v1 = v2;
  }

  // check for empty edges (like nothing in the viewport)
  if (ll_empty(edges)) {
    ll_delete(edges, NULL);
    edges = NULL;
  }

  return (edges);
}

/*
        Draw one scanline of a polygon given the scanline, the active edges,
        a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan(int scan, LinkedList *active, Image *src, Color c) {
  Edge *p1, *p2;

  // Loop over the list
  p1 = ll_head(active);
  while (p1) {
    // The edges have to come in pairs, draw from one to the next
    p2 = ll_next(active);
    if (!p2) {
      printf("bad bad bad (your edges are not coming in pairs)\n");
      break;
    }

    // If the xIntersect values are the same, don't draw anything.
    // Just go to the next pair.
    if (p2->xIntersect == p1->xIntersect) {
      p1 = ll_next(active);
      continue;
    }

    // Identify the starting column and clip to the left side of the image
    int startCol = (int)(p1->xIntersect + 0.5); // Round to nearest pixel column
    if (startCol < 0)
      startCol = 0; // Clip to left edge

    // Identify the ending column and clip to the right side of the image
    int endCol = (int)(p2->xIntersect + 0.5); // Round to nearest pixel column
    if (endCol >= src->cols)
      endCol = src->cols - 1; // Clip to right edge

    // Loop from start to end and color in the pixels
    for (int col = startCol; col <= endCol; col++) {
      if (col >= 0 && col < src->cols && scan >= 0 && scan < src->rows) {
        src->data[scan][col].rgb[0] = c.c[0];
        src->data[scan][col].rgb[1] = c.c[1];
        src->data[scan][col].rgb[2] = c.c[2];
        src->data[scan][col].a = 1.0; // Assuming full opacity
      }
    }

    // Move ahead to the next pair of edges
    p1 = ll_next(active);
  }
}

/*
         Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList(LinkedList *edges, Image *src, Color c) {
  LinkedList *active = NULL;
  LinkedList *tmplist = NULL;
  LinkedList *transfer = NULL;
  Edge *current;
  Edge *tedge;
  int scan = 0;

  active = ll_new();
  tmplist = ll_new();

  // get a pointer to the first item on the list and reset the current pointer
  current = ll_head(edges);

  // start at the first scanline and go until the active list is empty
  for (scan = current->yStart; scan < src->rows; scan++) {

    // grab all edges starting on this row
    while (current != NULL && current->yStart == scan) {
      ll_insert(active, current, compXIntersect);
      current = ll_next(edges);
    }
    // current is either null, or the first edge to be handled on some future
    // scanline

    if (ll_empty(active)) {
      break;
    }

    // if there are active edges
    // fill out the scanline
    fillScan(scan, active, src, c);

    // remove any ending edges and update the rest
    for (tedge = ll_pop(active); tedge != NULL; tedge = ll_pop(active)) {

      // keep anything that's not ending
      if (tedge->yEnd > scan) {
        float a = 1.0;

        // update the edge information with the dPerScan values
        tedge->xIntersect += tedge->dxPerScan;

        // adjust in the case of partial overlap
        if (tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1) {
          a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
          tedge->xIntersect = tedge->x1;
        } else if (tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1) {
          a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
          tedge->xIntersect = tedge->x1;
        }

        ll_insert(tmplist, tedge, compXIntersect);
      }
    }

    transfer = active;
    active = tmplist;
    tmplist = transfer;
  }

  // get rid of the lists, but not the edge records
  ll_delete(active, NULL);
  ll_delete(tmplist, NULL);

  return (0);
}

/*
        Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c) {
  LinkedList *edges = NULL;

  // set up the edge list
  edges = setupEdgeList(p, src);
  if (!edges)
    return;

  // process the edge list (should be able to take an arbitrary edge list)
  processEdgeList(edges, src, c);

  // clean up
  ll_delete(edges, (void (*)(const void *))free);

  return;
}

/****************************************
End Scanline Fill
*****************************************/
