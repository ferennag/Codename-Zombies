#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "engine/window.h"

typedef struct WindowContext {
    unsigned int width;
    unsigned int height;
    SDL_Window *window;
    SDL_GLContext *glContext;    
} WindowContext;

static WindowContext windowContext;

void fatal_exit() 
{
    Window_close();
    exit(EXIT_FAILURE);
}

void Window_create(const unsigned int width, const unsigned int height) 
{
    windowContext.width = width;
    windowContext.height = height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        printf("Unable to initialize SDL subsystems: %s\n", SDL_GetError());
        fatal_exit();
    }

    windowContext.window = SDL_CreateWindow(
        "Codename: Zombies",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL
    );

    if (!windowContext.window) {
        printf("Unable to create SDL window: %s\n", SDL_GetError());
        fatal_exit();
    }

    windowContext.glContext = SDL_GL_CreateContext(windowContext.window);

    if (!windowContext.glContext) {
        printf("Unable to load GL context: %s\n", SDL_GetError());
        fatal_exit();
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        printf("Unable to load GL context with Glad!\n");
        fatal_exit();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
}

void Window_close() 
{
    if(windowContext.glContext) {
        SDL_GL_DeleteContext(windowContext.glContext);
        windowContext.glContext = NULL;
    }

    if(windowContext.window) {
        SDL_DestroyWindow(windowContext.window);
        windowContext.window = NULL;
    }

    SDL_Quit();
}

void Window_update()
{
    SDL_GL_SwapWindow(windowContext.window);
}

float Window_aspect()
{
    return (float)windowContext.width / (float)windowContext.height;
}