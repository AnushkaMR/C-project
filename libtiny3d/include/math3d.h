// math3d.h

#ifndef MATH3D_H
#define MATH3D_H

#define M_PI 3.14159265358979323846

// 3D Vector structure
typedef struct {
    float x, y, z;
} vec3_t;

// 4x4 Matrix (column-major)
typedef struct {
    float m[4][4];
} mat4_t;

// Vector functions
vec3_t vec3_normalize_fast(vec3_t v);
vec3_t vec3_from_spherical(float r, float theta, float phi);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);
float vec3_dot(vec3_t a, vec3_t b);

// Matrix functions
mat4_t mat4_identity(void);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_rotate_xyz(float rx, float ry, float rz);
mat4_t mat4_mul(mat4_t a, mat4_t b);
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v);

#endif // MATH3D_H
