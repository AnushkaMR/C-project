#include "animation.h"

vec3_t bezier3(vec3_t p0, vec3_t p1, vec3_t p2, vec3_t p3, float t) {
    float u = 1 - t;
    float b0 = u*u*u, b1 = 3*u*u*t, b2 = 3*u*t*t, b3 = t*t*t;
    return (vec3_t){
        p0.x*b0 + p1.x*b1 + p2.x*b2 + p3.x*b3,
        p0.y*b0 + p1.y*b1 + p2.y*b2 + p3.y*b3,
        p0.z*b0 + p1.z*b1 + p2.z*b2 + p3.z*b3
    };
}
