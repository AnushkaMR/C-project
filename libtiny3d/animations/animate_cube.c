// animate_cube.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"    // For vec3_t, mat4_t, matrix ops
#include "renderer.h"  // For project_vertex

#define WIDTH   1920
#define HEIGHT  1080
#define NFRAMES 120

int main() {
    // 1. Create canvas
    canvas_t *cv = create_canvas(WIDTH, HEIGHT);
    if (!cv) {
        fprintf(stderr, "Error: Failed to create canvas\n");
        return 1;
    }

    // 2. Define cube vertices and edges
    vec3_t cube[8] = {
        {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
        {-1,-1, 1}, {1,-1, 1}, {1,1, 1}, {-1,1, 1}
    };
    int edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    // 3. Setup transformation matrices
    mat4_t scale_m     = mat4_scale(1.0f, 1.0f, 1.0f);
    mat4_t translate_m = mat4_translate(0.0f, 0.0f, -4.0f);
    mat4_t view = mat4_identity();

    // 4. Setup projection matrix
    float fov   = 90.0f * (M_PI / 180.0f);
    float aspect= (float)WIDTH / (float)HEIGHT;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tanf(fov * 0.5f);
    mat4_t proj = mat4_identity();
    proj.m[0][0] = f / aspect;
    proj.m[1][1] = f;
    proj.m[2][2] = far / (far - near);
    proj.m[2][3] = (-far * near) / (far - near);
    proj.m[3][2] = 1.0f;
    proj.m[3][3] = 0.0f;

    // 5. Animation loop
    for (int frame = 0; frame < NFRAMES; frame++) {
        printf("Rendering frame %d/%d\n", frame + 1, NFRAMES);

        // Clear canvas
        clear_canvas(cv);

        // Compute animated rotation
        float angle = frame * 0.05f;
        mat4_t rot = mat4_rotate_xyz(angle, angle * 0.6f, angle * 0.3f);

        // Combine model matrix: translate * rotate * scale
        mat4_t model = mat4_mul(translate_m, mat4_mul(rot, scale_m));

        // Project each vertex
        int xs[8], ys[8];
        float zs[8];
        for (int i = 0; i < 8; i++) {
            project_vertex(cube[i], model, view, proj, WIDTH, HEIGHT, &xs[i], &ys[i], &zs[i]);
        }

        // Draw each edge using anti-aliased lines
        for (int e = 0; e < 12; e++) {
            int a = edges[e][0], b = edges[e][1];
            draw_line_f(cv,
                        (float)xs[a], (float)ys[a],
                        (float)xs[b], (float)ys[b],
                        1.0f);  // constant thickness; adjust if needed
        }

        // Save canvas to PGM
        save_canvas_pgm(cv, frame);
    }

    // 6. Clean up
    destroy_canvas(cv);
    printf("Done. %d frames saved.\n", NFRAMES);

    return 0;
}
