// main.c

#include <stdio.h>
#include <math.h>
#include "canvas.h"

#define WIDTH 1920
#define HEIGHT 1080
#define PI 3.14159265

// Save canvas to a PGM image (for easy viewing)
void save_canvas_as_pgm(canvas_t *canvas, const char *filename) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "P2\n%d %d\n255\n", canvas->width, canvas->height);
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            int val = (int)(canvas->pixels[y][x] * 255.0f);
            fprintf(fp, "%d ", val);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main() {
    canvas_t *canvas = create_canvas(WIDTH, HEIGHT);

    float center_x = WIDTH / 2.0f;
    float center_y = HEIGHT / 2.0f;
    float radius = WIDTH * 0.45f; // reach close to edges

    for (int angle_deg = 0; angle_deg < 360; angle_deg += 15) {
        float angle_rad = angle_deg * (PI / 180.0f);
        float end_x = center_x + cosf(angle_rad) * radius;
        float end_y = center_y + sinf(angle_rad) * radius;

        draw_line_f(canvas, center_x, center_y, end_x, end_y, 1.5f);
    }

    save_canvas_as_pgm(canvas, "clockface.pgm");
    destroy_canvas(canvas);

    printf("Clock face lines saved to clockface.pgm\n");
    return 0;
}
