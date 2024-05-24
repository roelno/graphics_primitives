/*
  Bruce A. Maxwell
  Fall 2014

  Test function 1 for the graphics primitives assignment
  Enterpoop
 */

// edit the include files as necessary
#include "graphics.h"
#include "flood_fill.h"
#include "color.h"

// draw a box
static int box( Image *src, Color color, int x, int y, int dx, int dy ) {
  Line l;
  
  line_set2D( &l, x, y, x, y+dy );
  line_draw( &l, src, color );

  line_set2D( &l, x, y+dy, x + dx, y+dy );
  line_draw( &l, src, color );

  line_set2D( &l, x+dx, y+dy, x+dx, y );
  line_draw( &l, src, color );

  line_set2D( &l, x+dx, y, x, y );
  line_draw( &l, src, color );

  return(0);
}

// draw a few boxes, a circle, and some lines.
int main(int argc, char *argv[]) {
  Image *src;
  Color White;
  Color Red;
  Color Green;
  Color Blue;
  Point p;
  Circle circ;
  Line line;

  color_set(&White, 1.0, 1.0, 1.0);
  color_set(&Red, 0.9, 0.05, 0.05);
  color_set(&Green, 0.0, 1.0, 0.0);
  color_set(&Blue, 0.0, 0.0, 1.0);  // Blue fill color for boxes

  src = image_create(400, 600);

  // Draw and fill boxes with green border and blue fill
  box(src, Green, 100, 100, 100, 20);
  flood_fill(src, 105, 105, Blue); // Fill the first box

  box(src, Green, 100, 200, 100, 20);
  flood_fill(src, 105, 205, Blue); // Fill the second box

  box(src, Green, 150, 140, 70, 40);
  flood_fill(src, 155, 145, Blue); // Fill the third box

  box(src, Green, 160, 120, 10, 20);
  flood_fill(src, 165, 125, Blue); // Fill the fourth box

  box(src, Green, 160, 180, 10, 20);
  flood_fill(src, 165, 185, Blue); // Fill the fifth box

  // Draw and fill circle with red border and white fill
  point_set2D(&p, 260, 160);
  circle_set(&circ, p, 60);
  circle_draw(&circ, src, Red);
  flood_fill(src, 260, 160, White); // Fill the circle

  // Draw lines
  line_set2D(&line, 290, 210, 470, 300);
  line_draw(&line, src, Red);

  line_set2D(&line, 295, 205, 475, 280);
  line_draw(&line, src, Red);
  
  // Write the image to a file
  image_write(src, "test3a_flood_fill.ppm");

  // Free the image memory
  image_free(src);

  return 0;
}
