/*
  Bruce A. Maxwell
  Fall 2014

	Skeleton scanline fill algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "polygon.h"
#include "list.h"


/********************
Scanline Fill Algorithm
********************/

// define the struct here, because it is local to only this file
typedef struct tEdge {
	float x0, y0;                   /* start point for the edge */
	float x1, y1;                   /* end point for the edge */
	int yStart, yEnd;               /* start row and end row */
  float xIntersect, dxPerScan;    /* where the edge intersects the current scanline and how it changes */
	/* we'll add more here later */
  struct tEdge *next;
} Edge;


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
	Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	return(ea->yStart - eb->yStart);
}


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
	Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	if( ea->xIntersect < eb->xIntersect )
		return(-1);
	else if(ea->xIntersect > eb->xIntersect )
		return(1);

	return(0);
}

/*
	Allocates, creates, fills out, and returns an Edge structure given
	the inputs.

	Current inputs are just the start and end location in image space.
	Eventually, the points will be 3D and we'll add color and texture
	coordinates.
 */
static Edge *makeEdgeRec( Point start, Point end, Image *src)
{
	Edge *edge;
	float dscan = end.val[1] - start.val[1];

	/******
				 Your code starts here
	******/

	// Check if the starting row is below the image or the end row is
	// above the image and skip the edge if either is true

	// allocate an edge structure and set the x0, y0, x1, y1 values

	// turn on an edge only if the edge starts in the top half of it or
	// the lower half of the pixel above it.  In other words, round the
	// start y value to the nearest integer and assign it to
	// edge->yStart.

	// Turn off the edge if it starts in the lower half or the top half
	// of the next pixel.  In other words, round the end y value to the
	// nearest integer and subtract 1, assigning the result to
	// edge->yEnd.

	// Clip yEnd to the number of rows-1.

	// Calculate the slope, dxPerScan

	// Calculate xIntersect, adjusting for the fraction of the point in the pixel.
	// Scanlines go through the middle of pixels
	// Move the edge to the first scanline it crosses

	// adjust if the edge starts above the image
	// move the intersections down to scanline zero
	// if edge->y0 < 0
	//   update xIntersect
	//   update y0
  //   update x0
	//   update yStart


	// check for really bad cases with steep slopes where xIntersect has gone beyond the end of the edge

	// return the newly created edge data structure
	return( edge );
}


/*
	Returns a list of all the edges in the polygon in sorted order by
	smallest row.
*/
static LinkedList *setupEdgeList( Polygon *p, Image *src) {
	LinkedList *edges = NULL;
	Point v1, v2;
	int i;

	// create a linked list
	edges = ll_new();

	// walk around the polygon, starting with the last point
	v1 = p->vertex[p->nVertex-1];

	for(i=0;i<p->nVertex;i++) {
		
		// the current point (i) is the end of the segment
		v2 = p->vertex[i];

		// if it is not a horizontal line
		if( (int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5) ) {
			Edge *edge;

			// if the first coordinate is smaller (top edge)
			if( v1.val[1] < v2.val[1] )
				edge = makeEdgeRec( v1, v2, src );
			else
				edge = makeEdgeRec( v2, v1, src );

			// insert the edge into the list of edges if it's not null
			if( edge )
				ll_insert( edges, edge, compYStart );
		}
		v1 = v2;
	}

	// check for empty edges (like nothing in the viewport)
	if( ll_empty( edges ) ) {
		ll_delete( edges, NULL );
		edges = NULL;
	}

	return(edges);
}

/*
	Draw one scanline of a polygon given the scanline, the active edges,
	a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan( int scan, LinkedList *active, Image *src, Color c ) {
  Edge *p1, *p2;
  int i, f;

	// loop over the list
  p1 = ll_head( active );
  while(p1) {
		// the edges have to come in pairs, draw from one to the next
	  p2 = ll_next( active );
	  if( !p2 ) {
		  printf("bad bad bad (your edges are not coming in pairs)\n");
		  break;
	  }

		// if the xIntersect values are the same, don't draw anything.
		// Just go to the next pair.
	  if( p2->xIntersect == p1->xIntersect ) {
		  p1 = ll_next( active );
		  continue;
	  }

		/**** Your code goes here ****/
	  // identify the starting column
	  // clip to the left side of the image
	  // identify the ending column
	  // clip to the right side of the image
		// loop from start to end and color in the pixels

		// move ahead to the next pair of edges
	  p1 = ll_next( active );
  }

	return;
}

/* 
	 Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList( LinkedList *edges, Image *src, Color c ) {
	LinkedList *active = NULL;
	LinkedList *tmplist = NULL;
	LinkedList *transfer = NULL;
	Edge *current;
	Edge *tedge;
	int scan = 0;

	active = ll_new( );
	tmplist = ll_new( );

	// get a pointer to the first item on the list and reset the current pointer
	current = ll_head( edges );

	// start at the first scanline and go until the active list is empty
	for(scan = current->yStart;scan < src->rows;scan++ ) {

		// grab all edges starting on this row
		while( current != NULL && current->yStart == scan ) {
			ll_insert( active, current, compXIntersect );
			current = ll_next( edges );
		}
		// current is either null, or the first edge to be handled on some future scanline

		if( ll_empty(active) ) {
			break;
		}

		// if there are active edges
		// fill out the scanline
		fillScan( scan, active, src, c);

		// remove any ending edges and update the rest
		for( tedge = ll_pop( active ); tedge != NULL; tedge = ll_pop( active ) ) {

			// keep anything that's not ending
			if( tedge->yEnd > scan ) {
				float a = 1.0;

				// update the edge information with the dPerScan values
				tedge->xIntersect += tedge->dxPerScan;

				// adjust in the case of partial overlap
				if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}
				else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}

				ll_insert( tmplist, tedge, compXIntersect );
			}
		}

		transfer = active;
		active = tmplist;
		tmplist = transfer;

	}

	// get rid of the lists, but not the edge records
	ll_delete(active, NULL);
	ll_delete(tmplist, NULL);

	return(0);
}

/*
	Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c ) {
	LinkedList *edges = NULL;

	// set up the edge list
	edges = setupEdgeList( p, src );
	if( !edges )
		return;
	
	// process the edge list (should be able to take an arbitrary edge list)
	processEdgeList( edges, src, c);

	// clean up
	ll_delete( edges, (void (*)(const void *))free );

	return;
}

/****************************************
End Scanline Fill
*****************************************/
