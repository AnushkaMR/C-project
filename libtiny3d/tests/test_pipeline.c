// animate_cylinder.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"

#define WIDTH      1920
#define HEIGHT     1080
#define NFRAMES    1
#define SEGMENTS   32
#define RADIUS     1.0f
#define HEIGHT_CYL 2.0f

int main() {
    canvas_t *cv = create_canvas(WIDTH, HEIGHT);
    if (!cv) {
        fprintf(stderr, "Failed to create canvas\n");
        return 1;
    }

    // Generate cylinder vertices and edges
    int vcount = SEGMENTS * 2;
    vec3_t *verts = malloc(sizeof(vec3_t) * vcount);
    int ecount = SEGMENTS * 4;
    int (*edges)[2] = malloc(sizeof(int[2]) * ecount);

    for (int i = 0; i < SEGMENTS; i++) {
        float theta = 2 * M_PI * i / SEGMENTS;
        float x = cosf(theta) * RADIUS;
        float z = sinf(theta) * RADIUS;
        verts[i] = (vec3_t){x, HEIGHT_CYL * 0.5f, z};
        verts[i + SEGMENTS] = (vec3_t){x, -HEIGHT_CYL * 0.5f, z};
    }
    int idx = 0;
    for (int i = 0; i < SEGMENTS; i++) {
        int ni = (i + 1) % SEGMENTS;
        edges[idx][0] = i; edges[idx][1] = ni; idx++;
        edges[idx][0] = i + SEGMENTS; edges[idx][1] = ni + SEGMENTS; idx++;
        edges[idx][0] = i; edges[idx][1] = i + SEGMENTS; idx++;
        edges[idx][0] = ni; edges[idx][1] = ni + SEGMENTS; idx++;
    }

    mat4_t scale_m = mat4_identity();
    mat4_t translate_m = mat4_translate(0, 0, -5);
    mat4_t view = mat4_identity();

    float fov = 90.0f * (M_PI / 180.0f);
    float aspect = (float)WIDTH / HEIGHT;
    float min_n = 0.1f, max_f = 100.0f;
    float f = 1.0f / tanf(fov * 0.5f);

    mat4_t proj = mat4_identity();
    proj.m[0][0] = f / aspect;
    proj.m[1][1] = f;
    proj.m[2][2] = max_f / (max_f - min_n);
    proj.m[2][3] = (-max_f * min_n) / (max_f - min_n);
    proj.m[3][2] = 1;
    proj.m[3][3] = 0;

    for (int frame = 0; frame < NFRAMES; frame++) {
        printf("Frame %d/%d\n", frame + 1, NFRAMES);
        clear_canvas(cv);

        float ang = frame * 0.03f;
        mat4_t rot = mat4_rotate_xyz(ang, ang * 0.7f, ang * 0.4f);
        mat4_t model = mat4_mul(translate_m, mat4_mul(rot, scale_m));

        // Project vertices
        int xs[vcount], ys[vcount];
        float zs[vcount];
        for (int v = 0; v < vcount; v++) {
            project_vertex(verts[v], model, view, proj,
                            WIDTH, HEIGHT, &xs[v], &ys[v], &zs[v]);
        }

        // Draw edges with anti-aliased lines
        for (int e = 0; e < ecount; e++) {
            int a = edges[e][0], b = edges[e][1];
            draw_line_f(cv,
                        (float)xs[a], (float)ys[a],
                        (float)xs[b], (float)ys[b],
                        1.0f);
        }

        save_canvas_pgm(cv, frame);
    }

    destroy_canvas(cv);
    free(verts);
    free(edges);
    return 0;
}
