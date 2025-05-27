#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "./include/Renderer/Mesh.h" 
#include "./include/Utils/MeshReader.h"
#include "./include/Renderer/Matrix.h"

/*
 *
 * TODO: Finish programming the Camera matrix left off on transforming the old pipeline from
 * transform -> perspective -> draw
 *  INTO:
 * transform -> camera -> perspective -> draw
 *
 */


int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    Vec3 cameraPos = Vec3(0,0,0);
    Vec3 lookDir = Vec3(0,0,1);
    Vec3 up = Vec3(0, 1, 0);
    Vec3 target = Vec3_add(cameraPos, lookDir);

    Matrix4x4 matCamera = Matrix4x4_pointAt(cameraPos, target, up);

    Matrix4x4 matView = Matrix4x4_inverse(matCamera);

    int width = 800, height = 600;
    SDL_Window* win = SDL_CreateWindow("Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    Mesh mesh = Mesh_objToMesh("./assets/circle4.obj");
    
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
        Mesh_draw(&mesh, renderer, cameraPos, fov, aspect, near, far, width, height);

        Mesh_rotation(&mesh, Vec3(.01, 0.005, .01));

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}


