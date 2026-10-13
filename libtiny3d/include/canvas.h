#ifndef CANVAS_H
#define CANVAS_H

typedef struct {
    int width, height;
    float **pixels;
} canvas_t;

canvas_t *create_canvas(int width, int height);
void destroy_canvas(canvas_t *c);
void clear_canvas(canvas_t *c);
void draw_line_f(canvas_t *c, float x0, float y0, float x1, float y1, float thickness);
void save_canvas_pgm(canvas_t *c, int frame);

#endif // CANVAS_H
