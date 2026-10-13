// renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"

// Project a 3D point through model-view-projection into screen space
void project_vertex(vec3_t vertex, mat4_t model, mat4_t view, mat4_t proj,
                    int width, int height, int *x, int *y, float *depth);

// Check if a point is inside a circular viewport
int clip_to_circular_viewport(int width, int height, int x, int y);

// Render wireframe from vertices and edges using the transformation matrices
void render_wireframe(vec3_t *vertices, int v_count, int edges[][2], int e_count,
                      mat4_t model, mat4_t view, mat4_t proj,
                      int width, int height,
                      void (*draw_line)(int, int, int, int, unsigned char));

#endif
