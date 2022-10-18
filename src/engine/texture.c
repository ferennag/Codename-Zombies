#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include "engine/texture.h"
#include "data/hashmap.h"

static HashMap *cache = NULL;

void Texture_free(void *obj) 
{
    GLuint *texture = (GLuint *)obj;
    Texture_destroy(texture);
    free(texture);
}

GLuint Texture_load(const char *path) 
{
    if (!cache) {
        cache = HashMap_empty(Texture_free);
    }

    void *cachedTexture = HashMap_get(cache, path);

    if (cachedTexture) {
        return *((GLuint *) cachedTexture);
    }

    SDL_Surface *img = IMG_Load(path);

    if(!img) {
        printf("Unable to load texture: %s\n", path);
        return 0;
    }

    GLuint *texture = (GLuint *) malloc(sizeof(GLuint));
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(img);

    HashMap_put(cache, path, texture);

    return *texture;
}

void Texture_destroy(GLuint *texture)
{
    glDeleteTextures(1, texture);
}