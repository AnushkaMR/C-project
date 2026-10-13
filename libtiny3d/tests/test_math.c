#include <stdio.h>
#include <math.h>
#include "math3d.h"

// Multiply a point by a 4x4 matrix (with w = 1)
vec3_t multiply_point(mat4_t M, vec3_t v) {
    vec3_t r;
    float x = v.x, y = v.y, z = v.z;
    // calculate homogeneous w
    float w = M.m[3][0]*x + M.m[3][1]*y + M.m[3][2]*z + M.m[3][3];
    r.x = (M.m[0][0]*x + M.m[0][1]*y + M.m[0][2]*z + M.m[0][3]) / w;
    r.y = (M.m[1][0]*x + M.m[1][1]*y + M.m[1][2]*z + M.m[1][3]) / w;
    r.z = (M.m[2][0]*x + M.m[2][1]*y + M.m[2][2]*z + M.m[2][3]) / w;
    return r;
}

int main(void) {
    vec3_t cube[8] = {
        {-0.5f,-0.5f,-0.5f},{ 0.5f,-0.5f,-0.5f},
        { 0.5f, 0.5f,-0.5f},{-0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f},{ 0.5f,-0.5f, 0.5f},
        { 0.5f, 0.5f, 0.5f},{-0.5f, 0.5f, 0.5f}
    };

    mat4_t scale     = mat4_scale(2.0f, 2.0f, 2.0f);
    mat4_t rotate    = mat4_rotate_xyz(0.5f, 0.5f, 0.0f);
    mat4_t translate = mat4_translate(0.0f, 0.0f, -5.0f);

    // Build manual frustum projection matrix
    float left = -1, right = 1, bottom = -1, top = 1, near = 1, far = 10;
    mat4_t proj = {0};
    proj.m[0][0] = (2 * near) / (right - left);
    proj.m[1][1] = (2 * near) / (top - bottom);
    proj.m[0][2] = (right + left) / (right - left);
    proj.m[1][2] = (top + bottom) / (top - bottom);
    proj.m[2][2] = -(far + near) / (far - near);
    proj.m[2][3] = -(2 * far * near) / (far - near);
    proj.m[3][2] = -1;

    for (int i = 0; i < 8; i++) {
        vec3_t p = cube[i];
        p = multiply_point(scale, p);
        p = multiply_point(rotate, p);
        p = multiply_point(translate, p);

        vec3_t projected = multiply_point(proj, p);

        printf(
          "Vertex %d: world=(%.2f,%.2f,%.2f)  proj=(%.2f,%.2f)\n",
          i, p.x, p.y, p.z, projected.x, projected.y
        );
    }

    return 0;
}
