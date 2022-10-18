#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>

GLuint Shader_create_program(const char *vertex_shader, const char *fragment_shader);
void Shader_use(const GLuint shader);
void Shader_setUniformMat4(const GLuint program, const mat4 matrix, const char *name);