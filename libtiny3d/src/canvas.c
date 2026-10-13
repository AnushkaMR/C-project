#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "canvas.h"

static float clampf(float v, float a, float b) {
    return v < a ? a : v > b ? b : v;
}

canvas_t *create_canvas(int w, int h) {
    canvas_t *c = malloc(sizeof *c);
    c->width = w; c->height = h;
    c->pixels = malloc(h * sizeof *c->pixels);
    for (int y = 0; y < h; y++)
        c->pixels[y] = calloc(w, sizeof(float));
    return c;
}

void destroy_canvas(canvas_t *c) {
    for (int y = 0; y < c->height; y++)
        free(c->pixels[y]);
    free(c->pixels);
    free(c);
}

void clear_canvas(canvas_t *c) {
    for (int y = 0; y < c->height; y++)
        memset(c->pixels[y], 0, c->width * sizeof(float));
}

void set_pixel_f(canvas_t *c, float x, float y, float intensity) {
    int x0 = floorf(x), y0 = floorf(y);
    float dx = x - x0, dy = y - y0;
    float w[2][2] = {
        {(1-dx)*(1-dy), dx*(1-dy)},
        {(1-dx)*dy,      dx*dy}
    };
    for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) {
        int xi = x0 + j, yi = y0 + i;
        if (xi >= 0 && xi < c->width && yi >= 0 && yi < c->height) {
            float old = c->pixels[yi][xi];
            c->pixels[yi][xi] = clampf(old + intensity * w[i][j], 0.0f, 1.0f);
        }
    }
}

void draw_line_f(canvas_t *c, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0, dy = y1 - y0;
    float len = fmaxf(fabsf(dx), fabsf(dy)) * 2.0f;
    int steps = (int)len + 1;
    float sx = dx / steps, sy = dy / steps;
    for (int i = 0; i <= steps; i++) {
        float cx = x0 + sx * i, cy = y0 + sy * i;
        for (float oy = -thickness/2; oy <= thickness/2; oy += 0.5f)
            for (float ox = -thickness/2; ox <= thickness/2; ox += 0.5f)
                set_pixel_f(c, cx + ox, cy + oy, 1.0f);
    }
}

void save_canvas_pgm(canvas_t *c, int frame) {
    char buf[64];
    snprintf(buf, sizeof(buf), "frame%04d.pgm", frame);
    FILE *f = fopen(buf, "w");
    fprintf(f, "P2\n%d %d\n255\n", c->width, c->height);
    for (int y = 0; y < c->height; y++) {
        for (int x = 0; x < c->width; x++)
            fprintf(f, "%d ", (int)(clampf(c->pixels[y][x],0,1) * 255));
        fprintf(f, "\n");
    }
    fclose(f);
}
