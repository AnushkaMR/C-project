// math3d.c

#include <math.h>
#include "math3d.h"

// Identity matrix
mat4_t mat4_identity(void) {
    mat4_t m = {0};
    for (int i = 0; i < 4; i++) {
        m.m[i][i] = 1.0f;
    }
    return m;
}

// Scale matrix
mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    return m;
}

// Translation matrix
mat4_t mat4_translate(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

// Rotation matrix (Euler angles, radians) around X, Y, then Z axes
mat4_t mat4_rotate_xyz(float rx, float ry, float rz) {
    mat4_t rxm = mat4_identity();
    mat4_t rym = mat4_identity();
    mat4_t rzm = mat4_identity();

    float cx = cosf(rx);
    float sx = sinf(rx);
    float cy = cosf(ry);
    float sy = sinf(ry);
    float cz = cosf(rz);
    float sz = sinf(rz);

    // Rotation X
    rxm.m[1][1] = cx; rxm.m[1][2] = -sx;
    rxm.m[2][1] = sx; rxm.m[2][2] = cx;

    // Rotation Y
    rym.m[0][0] = cy; rym.m[0][2] = sy;
    rym.m[2][0] = -sy; rym.m[2][2] = cy;

    // Rotation Z
    rzm.m[0][0] = cz; rzm.m[0][1] = -sz;
    rzm.m[1][0] = sz; rzm.m[1][1] = cz;

    // Combine rotations Z * Y * X
    return mat4_mul(rzm, mat4_mul(rym, rxm));
}

// Matrix multiplication: result = a * b
mat4_t mat4_mul(mat4_t a, mat4_t b) {
    mat4_t result = {0};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++) {
                result.m[row][col] += a.m[row][k] * b.m[k][col];
            }
        }
    }
    return result;
}

// Multiply matrix by vector (assumes w=1)
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v) {
    vec3_t out;
    float x = v.x, y = v.y, z = v.z;

    out.x = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
    out.y = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
    out.z = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];

    return out;
}

// Optional: Fast inverse square root normalization
vec3_t vec3_normalize_fast(vec3_t v) {
    float length_sq = v.x*v.x + v.y*v.y + v.z*v.z;
    if (length_sq == 0.0f) return v;

    // Fast inverse square root approximation
    union { float f; int i; } conv = { length_sq };
    conv.i = 0x5f3759df - (conv.i >> 1);
    float inv_sqrt = conv.f * (1.5f - 0.5f * length_sq * conv.f * conv.f);

    vec3_t norm = { v.x * inv_sqrt, v.y * inv_sqrt, v.z * inv_sqrt };
    return norm;
}

// Dot product
float vec3_dot(vec3_t a, vec3_t b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Spherical linear interpolation (basic)
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    // Normalize inputs
    vec3_t v0 = vec3_normalize_fast(a);
    vec3_t v1 = vec3_normalize_fast(b);

    float dot = vec3_dot(v0, v1);
    if (dot > 0.9995f) {
        // Linear interpolation fallback for very close vectors
        vec3_t result = {
            v0.x + t*(v1.x - v0.x),
            v0.y + t*(v1.y - v0.y),
            v0.z + t*(v1.z - v0.z)
        };
        return vec3_normalize_fast(result);
    }

    if (dot < -1.0f) dot = -1.0f;
    else if (dot > 1.0f) dot = 1.0f;

    float theta = acosf(dot) * t;
    vec3_t v2 = {
        v1.x - v0.x * dot,
        v1.y - v0.y * dot,
        v1.z - v0.z * dot
    };
    v2 = vec3_normalize_fast(v2);

    vec3_t result = {
        v0.x * cosf(theta) + v2.x * sinf(theta),
        v0.y * cosf(theta) + v2.y * sinf(theta),
        v0.z * cosf(theta) + v2.z * sinf(theta)
    };
    return result;
}

// Convert spherical to Cartesian coordinates
vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;
    v.x = r * sinf(phi) * cosf(theta);
    v.y = r * sinf(phi) * sinf(theta);
    v.z = r * cosf(phi);
    return v;
}
