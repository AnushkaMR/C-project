#include <stdio.h>
#include <math.h>
#include "canvas.h"
#include "lighting.h"
#include "animation.h"
#include "math3d.h"
#include "renderer.h"

#define WIDTH   1920
#define HEIGHT  1080
#define FRAMES  120

static vec3_t cube[8] = {
   {-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},
   {-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}
};
static int cube_faces[6][4] = {
   {0,1,2,3},{4,5,6,7},{0,1,5,4},
   {2,3,7,6},{0,3,7,4},{1,2,6,5}
};

int face_visible(vec3_t *v, int *f, mat4_t m) {
    vec3_t p0 = mat4_mul_vec3(m, v[f[0]]);
    vec3_t p1 = mat4_mul_vec3(m, v[f[1]]);
    vec3_t p2 = mat4_mul_vec3(m, v[f[2]]);
    vec3_t u = {p1.x-p0.x, p1.y-p0.y, p1.z-p0.z};
    vec3_t w = {p2.x-p0.x, p2.y-p0.y, p2.z-p0.z};
    vec3_t n = {u.y*w.z - u.z*w.y, u.z*w.x - u.x*w.z, u.x*w.y - u.y*w.x};
    return (n.x*p0.x + n.y*p0.y + n.z*p0.z) < 0;
}

int main(){
    canvas_t *cv = create_canvas(WIDTH, HEIGHT);
    vec3_t light = {0.577f,0.577f,0.577f};
    vec3_t P0 = {-2,0,-6},P1 = {-2,2,-6}, P2 = { 2,2,-6}, P3 = {-2,0,-6}; 
    mat4_t view = mat4_identity();

    float fov = M_PI/2, asp = (float)WIDTH/HEIGHT, near=0.1f, far=100.0f;
    float f = 1.0f / tanf(fov/2);
    mat4_t proj = mat4_identity();
    proj.m[0][0] = f/asp; proj.m[1][1] = f;
    proj.m[2][2] = far/(far-near); proj.m[2][3] = -far*near/(far-near);
    proj.m[3][2] = 1; proj.m[3][3] = 0;

    for (int i=0; i<FRAMES; i++){
        float t = (float)i/(FRAMES-1);
        vec3_t pos = bezier3(P0,P1,P2,P3,t);
        clear_canvas(cv);

        mat4_t model = mat4_mul(
            mat4_translate(pos.x,pos.y,pos.z),
            mat4_rotate_xyz(t*M_PI*2, t*M_PI*2, 0)
        );

        int xs[8], ys[8];
        float zs[8];
        for (int v=0; v<8; v++)
            project_vertex(cube[v], model, view, proj, WIDTH, HEIGHT, &xs[v], &ys[v], &zs[v]);

        for (int f=0; f<6; f++){
            int *cf = cube_faces[f];
            if (!face_visible(cube, cf, model)) continue;
            for (int e=0; e<4; e++){
                int a=cf[e], b=cf[(e+1)&3];
                float L = line_light_intensity(cube[a],cube[b],light);
                draw_line_f(cv, xs[a], ys[a], xs[b], ys[b], L*2.5f);
            }
        }

        save_canvas_pgm(cv, i);
        printf("Frame %3d saved\n", i);
    }
    

    destroy_canvas(cv);
    return 0;
}

