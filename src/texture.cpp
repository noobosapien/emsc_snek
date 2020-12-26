#include "texture.h"
#include <iostream>
#include <malloc.h>

Texture::Texture():mTextureID(0), mWidth(0), mHeight(0){

}

Texture::~Texture(){

}

bool Texture::load(const std::string& filename, SDL_Renderer* renderer){

    SDL_Surface* sprite_surface = IMG_Load(filename.c_str());

    if(!sprite_surface){
        printf("Failed to load the surface: %s\n", IMG_GetError());
        return false;
    }

    SDL_Texture* sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);

    if(!sprite_texture){
        printf("Failed to load the texture: %s\n", IMG_GetError());
        return false;
    }

    SDL_QueryTexture(sprite_texture, NULL, NULL, &mWidth, &mHeight);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &mTextureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    flipTexMid32(sprite_surface);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite_surface);
    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(sprite_surface);
    
    return true;
}

void Texture::unload(){
    glDeleteTextures(1, &mTextureID);
}

void Texture::setActive(){
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}


void flipTexMid32(SDL_Surface* surface){

    if(surface){
        int h = surface->h;
        int pitch = surface->pitch / 4;

        Uint32* buffer = new Uint32[h * pitch/2];
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