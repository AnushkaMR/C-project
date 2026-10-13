// mainlight.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "lighting.h"
#include "animation.h"
#include "math3d.h"
#include "renderer.h"

#define WIDTH  1000
#define HEIGHT 1000
#define FRAMES 120

typedef struct {
    int a;
    int b;
} edge_t;

typedef struct {
    vec3_t position;
    vec3_t normal;
} vertex_t;

// Cube vertices with normals (24 vertices, 4 per face)
static vertex_t cube_vertices[24] = {
    {{-1, -1, -1}, {0, 0, -1}}, {{1, -1, -1},  {0, 0, -1}}, {{1, 1, -1},   {0, 0, -1}}, {{-1, 1, -1},  {0, 0, -1}},
    {{-1, -1, 1},  {0, 0, 1}},  {{1, -1, 1},   {0, 0, 1}},  {{1, 1, 1},    {0, 0, 1}},  {{-1, 1, 1},   {0, 0, 1}},
    {{-1, -1, -1}, {-1, 0, 0}}, {{-1, 1, -1},  {-1, 0, 0}}, {{-1, 1, 1},   {-1, 0, 0}}, {{-1, -1, 1},  {-1, 0, 0}},
    {{1, -1, -1},  {1, 0, 0}},  {{1, 1, -1},   {1, 0, 0}},  {{1, 1, 1},    {1, 0, 0}},  {{1, -1, 1},   {1, 0, 0}},
    {{-1, -1, -1}, {0, -1, 0}}, {{1, -1, -1},  {0, -1, 0}}, {{1, -1, 1},   {0, -1, 0}}, {{-1, -1, 1},  {0, -1, 0}},
    {{-1, 1, -1},  {0, 1, 0}},  {{1, 1, -1},   {0, 1, 0}},  {{1, 1, 1},    {0, 1, 0}},  {{-1, 1, 1},   {0, 1, 0}},
};

static edge_t cube_edges[] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {8,9},{9,10},{10,11},{11,8},
    {12,13},{13,14},{14,15},{15,12},
    {16,17},{17,18},{18,19},{19,16},
    {20,21},{21,22},{22,23},{23,20}
};
static const int cube_edge_count = sizeof(cube_edges) / sizeof(cube_edges[0]);

vec3_t cubic_bezier(vec3_t p0, vec3_t p1, vec3_t p2, vec3_t p3, float t) {
    float u = 1 - t;
    float tt = t*t;
    float uu = u*u;
    float uuu = uu * u;
    float ttt = tt * t;

    vec3_t result;
    result.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
    result.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
    result.z = uuu * p0.z + 3 * uu * t * p1.z + 3 * u * tt * p2.z + ttt * p3.z;
    return result;
}

void draw_filled_circle(canvas_t *c, int cx, int cy, int radius, float intensity) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && px < c->width && py >= 0 && py < c->height) {
                    float old = c->pixels[py][px];
                    c->pixels[py][px] = old + intensity > 1.0f ? 1.0f : old + intensity;
                }
            }
        }
    }
}

void render_cube_edges(canvas_t* canvas, mat4_t model, mat4_t view, mat4_t proj, vec3_t light) {
    float edge_intensity[cube_edge_count];
    for (int e = 0; e < cube_edge_count; e++) {
        vertex_t v0 = cube_vertices[cube_edges[e].a];
        vertex_t v1 = cube_vertices[cube_edges[e].b];

        mat4_t rot_only = model;
        rot_only.m[0][3] = rot_only.m[1][3] = rot_only.m[2][3] = 0.0f;

        vec3_t n0 = mat4_mul_vec3(rot_only, v0.normal);
        vec3_t n1 = mat4_mul_vec3(rot_only, v1.normal);

        n0 = vec3_normalize_fast(n0);
        n1 = vec3_normalize_fast(n1);

        vec3_t p0 = mat4_mul_vec3(model, v0.position);
        vec3_t p1 = mat4_mul_vec3(model, v1.position);
        vec3_t midpoint = {(p0.x + p1.x) * 0.5f, (p0.y + p1.y) * 0.5f, (p0.z + p1.z) * 0.5f};

        vec3_t light_dir = vec3_normalize_fast((vec3_t){light.x - midpoint.x, light.y - midpoint.y, light.z - midpoint.z});

        vec3_t lights[1] = {light_dir};
        float i0 = lambert_intensity(n0, lights, 1);
        float i1 = lambert_intensity(n1, lights, 1);
        edge_intensity[e] = (i0 + i1) * 0.5f;
    }

    int xs[24], ys[24];
    float zs[24];
    for (int i = 0; i < 24; i++) {
        project_vertex(cube_vertices[i].position, model, view, proj, WIDTH, HEIGHT, &xs[i], &ys[i], &zs[i]);
    }

    for (int e = 0; e < cube_edge_count; e++) {
        float brightness = edge_intensity[e];
        float thickness = 0.5f + brightness * 3.0f;
        draw_line_f(canvas, xs[cube_edges[e].a], ys[cube_edges[e].a], xs[cube_edges[e].b], ys[cube_edges[e].b], thickness);
    }
}

int main() {
    const vec3_t light = {0, 0, 0}; // Light source at origin
    vec3_t P0 = {-5, 0, -10}, P1 = {-5, 5, -10}, P2 = {5, 5, -10}, P3 = {-5, 0, -10};
   
    mat4_t view = mat4_identity();

    float fov = M_PI / 2;
    float aspect = (float)WIDTH / HEIGHT;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tanf(fov / 2);
    mat4_t proj = mat4_identity();
    proj.m[0][0] = f / aspect;
    proj.m[1][1] = f;
    proj.m[2][2] = far / (far - near);
    proj.m[2][3] = -far * near / (far - near);
    proj.m[3][2] = 1;
    proj.m[3][3] = 0;

    for (int frame = 0; frame < FRAMES; frame++) {
        float t1 = (float)frame / (FRAMES - 1);
        float t2 = t1 - 0.5f; // second cube offset by half the path
        if (t2 < 0.0f) t2 += 1.0f; // wrap around

        vec3_t pos1 = cubic_bezier(P0, P1, P2, P3, t1);
        vec3_t pos2 = cubic_bezier(P0, P1, P2, P3, t2);

        canvas_t* canvas = create_canvas(WIDTH, HEIGHT);
        clear_canvas(canvas);

        // First cube model: translate and rotate
        mat4_t model1 = mat4_mul(
            mat4_translate(pos1.x, pos1.y, pos1.z),
            mat4_rotate_xyz(t1 * 2 * M_PI, t1 * 2 * M_PI, 0)
        );

        // Second cube model: translate and rotate
        mat4_t model2 = mat4_mul(
            mat4_translate(pos2.x, pos2.y, pos2.z),
            mat4_rotate_xyz(t2 * 2 * M_PI, t2 * 2 * M_PI, 0)
        );

        render_cube_edges(canvas, model1, view, proj, light);
        render_cube_edges(canvas, model2, view, proj, light);

        // Draw light source as white ball at origin
        int light_x, light_y;
        float light_depth;
        project_vertex(light, mat4_identity(), view, proj, WIDTH, HEIGHT, &light_x, &light_y, &light_depth);
        draw_filled_circle(canvas, light_x, light_y, 6, 1.0f);

        save_canvas_pgm(canvas, frame);
        destroy_canvas(canvas);

        printf("Frame %3d saved\n", frame);
    }

    return 0;
}
