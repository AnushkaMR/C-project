#include <math.h>
#include "lighting.h"

// Lambert lighting: average dot product of edge direction with multiple lights
float lambert_intensity(vec3_t edge_dir, vec3_t* lights, int light_count) {
    float total = 0;
    for (int i = 0; i < light_count; i++) {
        float dot = edge_dir.x * lights[i].x + edge_dir.y * lights[i].y + edge_dir.z * lights[i].z;
        total += fmaxf(0.0f, dot);
    }
    return total / light_count;
}
