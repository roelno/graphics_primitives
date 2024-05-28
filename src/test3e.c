/*
  Bruce A. Maxwell
  Fall 2014

  Test function 1 for the graphics primitives assignment
  Enterpoop
 */

// edit the include files as necessary
#include "graphics.h"


// draw a ellipse
int main(int argc, char *argv[]) {
  Image *src;
  Color White;
  Color Red;
  Point p1, p2;
  Ellipse ellipse;
  Circle circle;

  color_set( &White, 1.0, 1.0, 1.0 );
  color_set( &Red, 0.9, 0.05, 0.05 );

  src = image_create( 400, 600 );

  point_set2D( &p1, 260, 160 );
  ellipse_set( &ellipse, p1, 60, 100 );
  ellipse_draw( &ellipse, src, White );
  ellipse_drawFill( &ellipse, src, Red );

  point_set2D( &p2, 100, 100 );
  circle_set( &circle, p2, 50 );
  circle_draw( &circle, src, Red );
  circle_drawFill( &circle, src, White );

  
  image_write( src, "test3e.ppm" );

  image_free( src );

  return(0);
}
