#define STB_IMAGE_IMPLEMENTATION
#include "img/stb_image.h"
#include "texture.h"
#include <iostream>
#include <malloc.h>

Texture::Texture():mTextureID(0), mWidth(0), mHeight(0){

}

Texture::~Texture(){

}

bool Texture::load(const std::string& filename){

    int nrChannels = 0;

    unsigned char* data = stbi_load(filename.c_str(), &mWidth, &mHeight, &nrChannels, 0);

    glGenTextures(1, &mTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        printf("Could not load the texture from file %s\n", filename.c_str());
    }

    stbi_image_free(data);
    
    return true;
}

void Texture::unload(){
    glDeleteTextures(1, &mTextureID);
}

void Texture::setActive(){
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::createFromSurface(SDL_Surface* surface){
    mWidth = surface->w;
    mHeight = surface->h;

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

bool Texture::loadFromGlyph(FT_Face& face){
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &mTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Rendering problem right here note: Needed: GL_LUMINANCE
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 
    face->glyph->bitmap.width,
    face->glyph->bitmap.rows,
    0,
    GL_LUMINANCE,
    GL_UNSIGNED_BYTE,
    face->glyph->bitmap.buffer);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    return true;
}

void flipTexMid32(SDL_Surface* surface){

    if(surface){
        int h = surface->h;
        int pitch = surface->pitch / 4;

        Uint32* buffer = new Uint32[h * pitch]; //pitch/2 gives out of bounds
        Uint32* pixels = static_cast<Uint32*>(surface->pixels);

        for(int row = 0; row < h; row++){
            for(int col = 0; col < pitch/2; col++){
                buffer[(row * pitch) + col] = pixels[(row * pitch) + col];
            }
        }

        for(int row = 0; row < h; row++){
            for(int col = pitch/2; col < pitch; col++){
                pixels[(row * pitch) + (col - pitch/2)] = pixels[(row * pitch) + col];
            }
        }

        for(int row = 0; row < h; row++){
            for(int col = 0; col < pitch/2; col++){
                pixels[(row * pitch) + (col + pitch/2)] = buffer[(row * pitch) + col];
            }
        }

        delete[] buffer;
    }

}