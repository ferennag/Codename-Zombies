#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#define BUFFER_SIZE 1024

char *Shader_load(const char *path, size_t *result_size)
{
    FILE *input = fopen(path, "r");
    if (NULL == input) {
        printf("Unable to load file: %s", path);
        return NULL;
    }

    size_t length = 0;
    char *result = NULL;
    char buffer[BUFFER_SIZE] = {0};

    while (fgets(buffer, BUFFER_SIZE, input)) {
        size_t str_length = strlen(buffer);
        if (NULL == result) {
            result = (char *) malloc(str_length * sizeof(char));
        } else {
            result = realloc(result, length + str_length);
        }

        strncpy((result + length), buffer, str_length);
        length += str_length;
        memset(buffer, 0, BUFFER_SIZE);
    }
    fclose(input);

    // We need to manually add the null terminator at the end of the string
    if (NULL != result) {
        result = realloc(result, length + 1);
        result[length] = '\0';
    }

    *result_size = length;
    return result;
}

GLuint Shader_compile(GLenum type, const char *shader_source_path) 
{
    size_t length = 0;
    char *shader_source = Shader_load(shader_source_path, &length);

    if (NULL == shader_source) {
        return 0;
    }

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (char const * const *) &shader_source, NULL);
    glCompileShader(shader);

    int success = 0;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error compiling the shader: %s\n", infoLog);
        return 0;
    }

    free(shader_source);
    return shader;
}

GLuint Shader_create_program(const char *vertex_shader, const char *fragment_shader)
{
    GLuint program = glCreateProgram();

    GLuint vs = Shader_compile(GL_VERTEX_SHADER, vertex_shader);
    if (!vs) {
        glDeleteProgram(program);
        return 0;
    }
    glAttachShader(program, vs);

    GLuint fs = Shader_compile(GL_FRAGMENT_SHADER, fragment_shader);
    if (!fs) {
        glDeleteProgram(program);
        return 0;
    }
    glAttachShader(program, fs);

    int success;
    char info_log[512];
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        printf("Error linking the shader program: %s\n", info_log);
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader_use(const GLuint shader) {
    glUseProgram(shader);
}

void Shader_setUniformMat4(const GLuint program, const mat4 matrix, const char *name)
{
    GLuint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *) matrix);
}