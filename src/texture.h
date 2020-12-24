#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <math.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_opengl.h>

#include <GLES2/gl2.h>
#include <GL/glfw.h>

class Texture{
public:
    Texture();
    ~Texture();

    bool load(const std::string& filename, SDL_Renderer* renderer);
    void unload();

    void setActive();

    int getWidth(){return mWidth;};
    int getHeight(){return mHeight;};

private:
    unsigned int mTextureID;
    int mWidth;
    int mHeight;

};

#endif