#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <stdlib.h>

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
    struct Vec3 camera = {.z = -1.f};
    struct Vec3 projectionDomain = {.x = 1.f, .y = 1.f};
    struct ModelArray models;
    
    struct Sphere sphere = {.radius = 0.5f};
    models_push_back(&models, tracer_get_sphere(&sphere));

    bool quit = false;
    SDL_Event e;
    while( !quit ){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ) quit = true;
        }

        /* GET MOUSE POSITION */
        SDL_GetMouseState(&context.mouseX, &context.mouseY);

        /* CLEAN BUFFERS */
        tracer_fill_rect(&vbuffer, (struct Vec3) {.x = 0.f, .y = 0.f}, context.width, context.height, GRAPHICS_BLACK);

        /* APPLICATION DRAWING */
        for(unsigned int i = 0; i < context.height; i++){
            struct Vec3 direction = {.x = i * 2.f * projectionDomain.y / context.height};
            for(unsigned int j = 0; j < context.width; j++){
                direction.y = j * 2.f * projectionDomain.x / context.width;
                struct Ray ray = {.origin = camera, .direction = direction};
                vbuffer.buffer[i * context.width + j] = 
                    tracer_color_from_vec3(tracer_get_pixel_color(ray, &models, 1));
            }
        }

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