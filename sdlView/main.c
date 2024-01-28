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
        const struct Vec3 center = {.x = context.width / 2.f, .y = context.height / 2.f};
        const struct Vec3 mousePosition = {.x = context.mouseX, .y = context.mouseY};
        const float radius = 100.f;
        
        const struct Vec3 rayOrigin = {.x = 0.f, .y = context.height / 2.f}; 
        const struct Vec3 direction = vec3_normalize(vec3_sub(mousePosition, rayOrigin));
        const struct Ray ray = {.origin = rayOrigin, .direction = direction};
        
        tracer_fill_circle(&vbuffer, center, radius, GRAPHICS_PURPLE);

        tracer_fill_circle(&vbuffer, mousePosition, 20, GRAPHICS_YELLOW);
        tracer_fill_circle(&vbuffer, rayOrigin, 20, GRAPHICS_PURPLE);
        tracer_stroke_line(&vbuffer, rayOrigin, vec3_add(rayOrigin, vec3_scale(direction, 100.f)), GRAPHICS_LIGHT_BLUE);

        struct Sphere sphere = {.center = center, .radius = radius};
        struct Vec3 intersection;
        enum IntersectionState state = tracer_sphere_collision(ray, &sphere, &intersection);
        printf("\rINTERSECTION STATE: %s", (state == INTESECT_VALID) ? "VALID" : "ERROR");
        tracer_fill_circle(&vbuffer, intersection, 20, GRAPHICS_LIGHT_BLUE);

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