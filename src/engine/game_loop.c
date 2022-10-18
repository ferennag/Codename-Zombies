#include <stdbool.h>
#include <SDL2/SDL.h>
#include "cglm/cglm.h"
#include "engine/window.h"
#include "engine/game_loop.h"
#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/box.h"
#include "engine/camera.h"
#include "engine/time.h"
#include "data/list.h"

typedef struct GameState {
    bool running;
    mat4 projection;
    GLuint program;
    List *scene;
    Camera *camera;
} GameState;

static GameState state;
static bool running = true;

void GameLoop_handleKeydown(SDL_Event *e)
{
    switch (e->key.keysym.sym)
    {
        case SDLK_q:
        case SDLK_ESCAPE:
            running = false;
            break;

        case SDLK_UP:
        case SDLK_w:
            Camera_move_forward(state.camera, 0.5f);
            break;

        case SDLK_DOWN:
        case SDLK_s:
            Camera_move_backward(state.camera, 0.5f);
            break;

        case SDLK_LEFT:
        case SDLK_a:
            Camera_strafe_left(state.camera, 0.5f);
            break;

        case SDLK_RIGHT:
        case SDLK_d:
            Camera_strafe_right(state.camera, 0.5f);
            break;
        
        default:
            break;
    }
}

void GameLoop_handleMouseMotion(SDL_Event *e)
{
    const float sensitivity = 0.1f;
    float dx = e->motion.xrel * sensitivity;
    float dy = e->motion.yrel * sensitivity;

    state.camera->yaw += dx;
    state.camera->pitch += dy;

    if(state.camera->pitch > 89.0f) {
        state.camera->pitch = 89.0f;
    }

    if(state.camera->pitch < -89.0f) {
        state.camera->pitch = -89.0f;
    }
}

void GameLoop_pollEvents()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch (e.type)
        {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                GameLoop_handleKeydown(&e);
                break;

            case SDL_MOUSEMOTION:
                GameLoop_handleMouseMotion(&e);
                break;
            
            default:
                break;
        }
    }
}

void GameLoop_beforeStart()
{
    glm_perspective(glm_rad(45.0f), Window_aspect(), 0.1f, 100.0f, state.projection);

    state.program = Shader_create_program("shaders/vertex.glsl", "shaders/fragment.glsl");
    LCallbacks callbacks = {.lfree = Box_destroy};
    state.scene = List_empty(&callbacks);
    List_append(state.scene, Box_create());
    state.camera = Camera_create();
}

void GameLoop_afterFinish()
{
    List_destroy(state.scene);
    Camera_destroy(state.camera);
}

void GameLoop_update()
{
    Camera_lookat(state.camera);

    glUseProgram(state.program);
    Shader_setUniformMat4(state.program, state.projection, "projection");
    Shader_setUniformMat4(state.program, state.camera->view, "view");

    Node *it = List_first(state.scene);
    while(it != state.scene->tail) {
        Box *box = (Box *) it->data;
        glBindVertexArray(box->vao);
        // glm_rotate_x(box->model, glm_rad(1), box->model);
        // glm_rotate_y(box->model, glm_rad(1), box->model);
        Shader_setUniformMat4(state.program, box->model, "model"); 
        Box_render(box);

        it = it->next;
    }

    glUseProgram(0);
}

void GameLoop_run()
{
    Uint64 msPerFrame = 1000 / 60;
    Uint64 frameStart = 0;
    
    GameLoop_beforeStart();
    while(running) {
        frameStart = Time_getTicks();

        GameLoop_pollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GameLoop_update();
        Window_update();

        Uint64 frameTime = Time_getTicks() - frameStart;
        if (frameTime < msPerFrame) {
            Time_delay(msPerFrame - frameTime);
        }
    }
    GameLoop_afterFinish();
}