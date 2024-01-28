#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "tracer.h"
#include "graphics.h"

#define INIT_WINDOW_ERROR 1

static struct {
    unsigned int width, height;
    unsigned int time;
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

    bool quit = false;
    SDL_Event e;
    while( !quit ){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ) quit = true;
        }

        /* CLEAN BUFFERS */
        tracer_fill_rect(&vbuffer, (struct Vec3) {.x = 0.f, .y = 0.f}, context.width, context.height, GRAPHICS_BLACK);

        /* APPLICATION DRAWING */
        printf("\rcurrent float: %f", sinf(context.time));
        tracer_fill_rect(&vbuffer, (struct Vec3) {.x = 0.f, .y = 0.f}, (unsigned int)fabs(sin(context.time / 50.f) * 200), 100, GRAPHICS_WHITE);

        tracer_fill_circle(&vbuffer, (struct Vec3) {.x = context.width / 2.f, .y = context.height / 2.f}, 100, GRAPHICS_PURPLE);

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