#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "./include/Renderer/Mesh.h" 
#include "./include/Utils/MeshReader.h"
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    Vec3 camera = Vec3(0,0,0);

    int width = 800, height = 600;
    SDL_Window* win = SDL_CreateWindow("Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    Mesh mesh = Mesh_objToMesh("./assets/circle4.obj");
    Vertex vertices[8] = {
        Vertex(Vec3_init( 1,  1,  1), Color(255, 0, 0, 255)),  // Red (positive x, y, z)
        Vertex(Vec3_init(-1,  1,  1), Color(0, 255, 0, 255)),  // Green (negative x, positive y, z)
        Vertex(Vec3_init(-1, -1,  1), Color(0, 0, 255, 255)),  // Blue (negative x, negative y, z)
        Vertex(Vec3_init( 1, -1,  1), Color(255, 255, 0, 255)), // Yellow (positive x, negative y, z)
        Vertex(Vec3_init( 1,  1, -1), Color(255, 165, 0, 255)), // Orange (positive x, y, negative z)
        Vertex(Vec3_init(-1,  1, -1), Color(128, 0, 128, 255)), // Purple (negative x, positive y, negative z)
        Vertex(Vec3_init(-1, -1, -1), Color(0, 255, 255, 255)), // Cyan (negative x, negative y, negative z)
        Vertex(Vec3_init( 1, -1, -1), Color(255, 0, 255, 255))  // Magenta (positive x, negative y, negative z)
    };
    Tri3D tris[12] = {
        // Front face (+Z)
        Tri3D(vertices[0], vertices[1], vertices[2]),
        Tri3D(vertices[0], vertices[2], vertices[3]),

        // Back face (-Z)
        Tri3D(vertices[4], vertices[6], vertices[5]),
        Tri3D(vertices[4], vertices[7], vertices[6]),

        // Left face (-X)
        Tri3D(vertices[1], vertices[5], vertices[6]),
        Tri3D(vertices[1], vertices[6], vertices[2]),

        // Right face (+X)
        Tri3D(vertices[0], vertices[3], vertices[7]),
        Tri3D(vertices[0], vertices[7], vertices[4]),

        // Top face (+Y)
        Tri3D(vertices[0], vertices[4], vertices[5]),
        Tri3D(vertices[0], vertices[5], vertices[1]),

        // Bottom face (-Y)
        Tri3D(vertices[3], vertices[2], vertices[6]),
        Tri3D(vertices[3], vertices[6], vertices[7])
    };
    /*
    Mesh mesh = {
        .tris = tris,
        .count = 12,
        .centroid = Vec3(0, 0, 0)
    };
*/
    // Move cube back along z-axis
    Vec3 move_back = {0, 0, 5};
    Vec3 rotate_vec = {20, 15, 20};
    Mesh_translate(&mesh, move_back);

    float fov = M_PI / 2.0f;
    float aspect = (float)width / (float)height;
    float near = 0.1f, far = 100.0f;
    int flip = 0;
    int running = 1;
    while (running) {
        SDL_Event e;
        //handle input
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {
                running = 0;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                     case SDLK_DOWN:
                         printf("\nmove down");
                         Mesh_translate(&mesh, Vec3(0,0,.5));
                         break;
                     case SDLK_UP:
                         printf("\nmove up");
                         Mesh_translate(&mesh, Vec3(0,0,-.5));
                         break;
                     case SDLK_w:
                         Mesh_rotation(&mesh, Vec3(.5, 0, 0));
                         break;
                     case SDLK_s:
                         Mesh_rotation(&mesh, Vec3(0, .5, 0));
                         break;
                     case SDLK_a:
                         Mesh_rotation(&mesh, Vec3(0, 0, .5));
                         break;
                     case SDLK_d:
                         Mesh_rotation(&mesh, rotate_vec);
                         break;
                     case SDLK_q:
                         Mesh_rotation(&mesh, rotate_vec);
                         break;
                     case SDLK_e:
                         Mesh_rotation(&mesh, rotate_vec);
                         break;                
                }
            }
        }
        Mesh_draw(&mesh, renderer, camera, fov, aspect, near, far, width, height);

        Mesh_rotation(&mesh, Vec3(.01, 0.005, .01));

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}


