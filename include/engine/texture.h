#pragma once

#include <glad/glad.h>

GLuint Texture_load(const char *path);
void Texture_destroy(GLuint *texture);