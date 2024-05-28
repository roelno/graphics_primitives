#include "graphics.h"
#include "point.h"
#include "line.h"
#include "image.h"
#include "color.h"
#include "flood_fill.h"
#include <math.h>
#include <stdio.h>

// Function to project 3D point to 2D with a simple perspective projection
void project_point_perspective(Point *p3d, Point *p2d, double d) {
    p2d->val[0] = p3d->val[0] * d / (d + p3d->val[2]);
    p2d->val[1] = p3d->val[1] * d / (d + p3d->val[2]);
}

// Function to draw a filled parallelogram
void draw_filled_parallelogram(Point *p1, Point *p2, Point *p3, Point *p4, Image *image, Color outline_color, Color fill_color) {
    // Drawing the outline
    Line line1, line2, line3, line4;
    line_set(&line1, *p1, *p2);
    line_set(&line2, *p2, *p3);
    line_set(&line3, *p3, *p4);
    line_set(&line4, *p4, *p1);
    line_draw(&line1, image, outline_color);
    line_draw(&line2, image, outline_color);
    line_draw(&line3, image, outline_color);
    line_draw(&line4, image, outline_color);

    // Calculate the centroid of the parallelogram to use as the fill point
    int x_fill = (int)((p1->val[0] + p2->val[0] + p3->val[0] + p4->val[0]) / 4);
    int y_fill = (int)((p1->val[1] + p2->val[1] + p3->val[1] + p4->val[1]) / 4);
    flood_fill(image, x_fill, y_fill, fill_color);
}

// Function to draw a 3D box
void draw_box(Image *image) {
    Color outline_color = {0.0, 0.0, 0.0}; // Black color for the outline
    Color face_color1 = {0.6, 0.6, 0.6};   // Gray color for the front face
    Color face_color2 = {0.5, 0.5, 0.5};   // Slightly darker gray for the top face
    Color face_color3 = {0.4, 0.4, 0.4};   // Even darker gray for the side face

    double d = 500; // Distance from the camera for perspective projection

    // Define key points of the box in 3D
    Point p1, p2, p3, p4, p5, p6, p7, p8;
    point_set3D(&p1, 300, 300, 0);   // Front bottom left
    point_set3D(&p2, 500, 300, 0);   // Front bottom right
    point_set3D(&p3, 500, 500, 0);   // Back bottom right
    point_set3D(&p4, 300, 500, 0);   // Back bottom left
    point_set3D(&p5, 300, 300, 200); // Front top left
    point_set3D(&p6, 500, 300, 200); // Front top right
    point_set3D(&p7, 500, 500, 200); // Back top right
    point_set3D(&p8, 300, 500, 200); // Back top left

    // Project the points to 2D with perspective
    Point p2d[8];
    project_point_perspective(&p1, &p2d[0], d);
    project_point_perspective(&p2, &p2d[1], d);
    project_point_perspective(&p3, &p2d[2], d);
    project_point_perspective(&p4, &p2d[3], d);
    project_point_perspective(&p5, &p2d[4], d);
    project_point_perspective(&p6, &p2d[5], d);
    project_point_perspective(&p7, &p2d[6], d);
    project_point_perspective(&p8, &p2d[7], d);

    // Draw and fill the faces of the box
    draw_filled_parallelogram(&p2d[0], &p2d[1], &p2d[5], &p2d[4], image, outline_color, face_color1); // Front face
    draw_filled_parallelogram(&p2d[4], &p2d[5], &p2d[6], &p2d[7], image, outline_color, face_color2); // Top face
    draw_filled_parallelogram(&p2d[1], &p2d[2], &p2d[6], &p2d[5], image, outline_color, face_color3); // Right face
    draw_filled_parallelogram(&p2d[0], &p2d[3], &p2d[7], &p2d[4], image, outline_color, face_color3); // Left face
    draw_filled_parallelogram(&p2d[3], &p2d[2], &p2d[6], &p2d[7], image, outline_color, face_color2); // Back face
}

int main() {
    // Create an image to draw on
    Image *image = image_create(800, 800);
    if (!image) {
        fprintf(stderr, "Could not create image\n");
        return 1;
    }

    // Fill the image with white color
    image_fillrgb(image, 1, 1, 1);

    // Draw the 3D box
    draw_box(image);

    // Write the image to a file
    image_write(image, "box.ppm");

    // Free the image
    image_free(image);

    return 0;
}
