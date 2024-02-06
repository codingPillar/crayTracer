#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "SDL_keycode.h"
#include "tracer.h"
#include "graphics.h"
#include "sphere.h"

#define INIT_WINDOW_ERROR 1

static struct {
    unsigned int width, height;
    unsigned int time;
    int mouseX, mouseY;
} context;

int main(void){
    printf("SCENE RAY TRACER\n");

    context.width = 1080;
    context.height = 720;
    context.time = 0;

    SDL_Window *window;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return INIT_WINDOW_ERROR;
    }
    window = SDL_CreateWindow("Scene Ray Tracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
       context.width, context.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if(!window){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return INIT_WINDOW_ERROR;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STREAMING, context.width, context.height);

    struct VBuffer vbuffer;
    tracer_init_buffer(&vbuffer, context.width, context.height);

    /* INITIALIZE APPLICATION MODELS */
    struct Scene scene = {
        .camera = {.position = {.z = 1.f}}, .projectionDomain = {.x = (float)context.width / context.height, .y = 1.0}
    };
    
    tracer_lookat(&scene.camera, (struct Vec3){0});
    struct Sphere sphere1 = {.radius = 0.5f, .center = {.z = -1.f}, .color = {.x = 0.5f, .z = 1.f}};
    struct Sphere sphere2 = {.radius = 0.5f, .center = {.x = -0.5f, .z = -2.f}, .color = {.x = 1.f, .z = .5f}};
    models_push_back(&scene.models, tracer_get_sphere(&sphere1));
    models_push_back(&scene.models, tracer_get_sphere(&sphere2));

    bool quit = false;
    SDL_Event e;
    while( !quit ){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE ) quit = true;
            /* UPDATE LOGIC */
            else if(e.key.keysym.sym == SDLK_a){
                scene.camera.position.x -= 0.05f;
            }else if(e.key.keysym.sym == SDLK_d){
                scene.camera.position.x += 0.05f;
            }
        }

        /* CLEAN BUFFERS */
        tracer_fill_rect(&vbuffer, (struct Vec3) {.x = 0.f, .y = 0.f}, context.width, context.height, GRAPHICS_BLACK);

        /* GET MOUSE POSITION */
        SDL_GetMouseState(&context.mouseX, &context.mouseY);

        /* APPLICATION DRAWING */
        tracer_draw_scene(&scene, &vbuffer);

        /* TEXTURE DRAWING */
        int pitch;
        uint32_t *pixels;
        SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
        for(unsigned int i = 0; i < context.width * context.height; i++) pixels[i] = vbuffer.buffer[i];
        SDL_UnlockTexture(texture);
 
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        context.time++;
    }

    return 0;
}