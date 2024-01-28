#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "SDL_render.h"
#include "tracer.h"

#define INIT_WINDOW_ERROR 1

int main(void){
    printf("SCENE RAY TRACER\n");

    unsigned int width = 1080, height = 720;

    SDL_Window *window;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return INIT_WINDOW_ERROR;
    }
    window = SDL_CreateWindow("Scene Ray Tracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if(!window){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return INIT_WINDOW_ERROR;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STREAMING, width, height);

    bool quit = false;
    SDL_Event e;
    while( !quit ){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ) quit = true;
        }

        int pitch;
        uint32_t *pixels;
        SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
        for(unsigned int i = 0; i < width * height; i++){
            pixels[i] = 0xFFFF00FF;
        }
        SDL_UnlockTexture(texture);
 
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        //SDL_Delay(10);
    }

    return 0;
}