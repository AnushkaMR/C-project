// animate_soccer.c  – wire‑frame truncated‑icosahedron (soccer ball)
// draws with canvas_t / draw_line_f() from canvas.c (anti‑aliased)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"

#define WIDTH   1920
#define HEIGHT  1080
#define NFRAMES 120

/* ---- constant vertex data ---- */
#define C0 0.809016994f
#define C1 1.618033988f
#define C2 1.809016994f
#define C3 2.118033988f
#define C4 2.427050983f
static const vec3_t verts[60] = {
 /* 0‑11  */ { 0.5f,0, C4}, { 0.5f,0,-C4}, {-0.5f,0, C4}, {-0.5f,0,-C4},
              { C4, 0.5f,0}, { C4,-0.5f,0}, {-C4, 0.5f,0}, {-C4,-0.5f,0},
              {0, C4, 0.5f}, {0, C4,-0.5f}, {0,-C4, 0.5f}, {0,-C4,-0.5f},
 /* 12‑23 */ { 1, C0, C3},{ 1, C0,-C3},{ 1,-C0, C3},{ 1,-C0,-C3},
              {-1, C0, C3},{-1, C0,-C3},{-1,-C0, C3},{-1,-C0,-C3},
              { C3, 1, C0},{ C3, 1,-C0},{ C3,-1, C0},{ C3,-1,-C0},
 /* 24‑35 */ {-C3, 1, C0},{-C3, 1,-C0},{-C3,-1, C0},{-C3,-1,-C0},
              { C0, C3, 1},{ C0, C3,-1},{ C0,-C3, 1},{ C0,-C3,-1},
              {-C0, C3, 1},{-C0, C3,-1},{-C0,-C3, 1},{-C0,-C3,-1},
 /* 36‑47 */ { 0.5f, C1, C2},{ 0.5f, C1,-C2},{ 0.5f,-C1, C2},{ 0.5f,-C1,-C2},
              {-0.5f, C1, C2},{-0.5f, C1,-C2},{-0.5f,-C1, C2},{-0.5f,-C1,-C2},
              { C2, 0.5f, C1},{ C2, 0.5f,-C1},{ C2,-0.5f, C1},{ C2,-0.5f,-C1},
 /* 48‑59 */ {-C2, 0.5f, C1},{-C2, 0.5f,-C1},{-C2,-0.5f, C1},{-C2,-0.5f,-C1},
              { C1, C2,0.5f},{ C1, C2,-0.5f},{ C1,-C2,0.5f},{ C1,-C2,-0.5f},
              {-C1, C2,0.5f},{-C1, C2,-0.5f},{-C1,-C2,0.5f},{-C1,-C2,-0.5f}
};

/* 32 faces (hexagons first, then pentagons padded w/ -1) */
static const int faces[32][6] = {
    {0,2,18,42,38,14},{1,3,17,41,37,13},{2,0,12,36,40,16},{3,1,15,39,43,19},
    {4,5,23,47,45,21},{5,4,20,44,46,22},{6,7,26,50,48,24},{7,6,25,49,51,27},
    {8,9,33,57,56,32},{9,8,28,52,53,29},{10,11,31,55,54,30},{11,10,34,58,59,35},
    {12,44,20,52,28,36},{13,37,29,53,21,45},{14,38,30,54,22,46},{15,47,23,55,31,39},
    {16,40,32,56,24,48},{17,49,25,57,33,41},{18,50,26,58,34,42},{19,43,35,59,27,51},
    {0,14,46,44,12,-1},{1,13,45,47,15,-1},{2,16,48,50,18,-1},{3,19,51,49,17,-1},
    {4,21,53,52,20,-1},{5,22,54,55,23,-1},{6,24,56,57,25,-1},{7,27,59,58,26,-1},
    {8,32,40,36,28,-1},{9,29,37,41,33,-1},{10,30,38,42,34,-1},{11,35,43,39,31,-1}
};

/* ---- edge list construction ---- */
int build_edges(int out[90][2]){
    int cnt=0;
    for(int f=0;f<32;f++){
        int prev = faces[f][5]>=0 ? faces[f][5] : faces[f][4];
        for(int i=0;i<6;i++){
            int cur=faces[f][i]; if(cur<0) break;
            int a=prev<cur?prev:cur, b=prev<cur?cur:prev;
            int dup=0; for(int e=0;e<cnt;e++) if(out[e][0]==a && out[e][1]==b){dup=1;break;}
            if(!dup){ out[cnt][0]=a; out[cnt][1]=b; cnt++; }
            prev=cur;
        }
    }
    return cnt; /* should be 90 */
}

/* ---- canvas drawing helpers ---- */
#include "canvas.h"
void draw_edge(canvas_t *cv,int ax,int ay,int bx,int by){
    draw_line_f(cv,(float)ax,(float)ay,(float)bx,(float)by,1.0f);
}

int main(void){
    /* create canvas */
    canvas_t *cv = create_canvas(WIDTH,HEIGHT);

    /* build unique edge list */
    int edges[90][2]; int ecount = build_edges(edges);

    /* basic camera / projection */
    mat4_t view = mat4_identity();
    mat4_t trans = mat4_translate(0,0,-5);

    float fov = 90*(M_PI/180.0f), asp=(float)WIDTH/HEIGHT;
    float near=0.1f, far=100; float f = 1.0f/tanf(fov*0.5f);
    mat4_t proj = mat4_identity();
    proj.m[0][0]=f/asp; proj.m[1][1]=f;
    proj.m[2][2]=far/(far-near); proj.m[2][3]=-far*near/(far-near);
    proj.m[3][2]=1; proj.m[3][3]=0;

    /* animation loop */
    for(int frame=0; frame<NFRAMES; ++frame){
        clear_canvas(cv);

        float ang = frame*0.03f;
        mat4_t model = mat4_mul(trans, mat4_rotate_xyz(ang, ang*0.6f, ang*0.3f));

        /* project 60 vertices */
        int sx[60], sy[60]; float sz[60];
        for(int v=0; v<60; ++v)
            project_vertex(verts[v],model,view,proj,WIDTH,HEIGHT,&sx[v],&sy[v],&sz[v]);

        /* draw 90 edges */
        for(int e=0; e<ecount; ++e){
            int a=edges[e][0], b=edges[e][1];
            draw_edge(cv, sx[a], sy[a], sx[b], sy[b]);
        }

        save_canvas_pgm(cv,frame);
        printf("Saved soccer%04d.pgm\n",frame);
    }

    destroy_canvas(cv);
    return 0;
}
