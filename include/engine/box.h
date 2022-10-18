#pragma once

#include <glad/glad.h>

typedef struct Box {
    GLuint vao;
    GLuint vbo;
    GLuint texture;
    mat4 model;
} Box;

Box *Box_create();
void Box_destroy(void *obj);
void Box_render(Box *box);