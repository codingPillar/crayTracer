#include <stdio.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "tracer.h"

#define INIT_WINDOW_ERROR 1

int main(void){
    printf("SCENE RAY TRACER\n");

    unsigned int width = 1080, height = 720;

    SDL_Window *window;
    SDL_GLContext context;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return INIT_WINDOW_ERROR;
    }
    window = SDL_CreateWindow("Cprocessing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if(!window){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return INIT_WINDOW_ERROR;
    }
    context = SDL_GL_CreateContext(window);
    if(!context){
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return INIT_WINDOW_ERROR;
    }
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK){
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return INIT_WINDOW_ERROR;
    }
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OPENGL VERSION: %d.%d\n", major, minor);

    bool quit = false;
    SDL_Event e;
    while( !quit ){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ) quit = true;
        }

        SDL_GL_SwapWindow(window);
        //SDL_Delay(10);
    }

    return 0;
}