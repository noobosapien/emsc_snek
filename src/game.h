#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_opengl.h>

#include <GLES2/gl2.h>
#include <GL/glfw.h>

#include "actor.h"
#include "shader.h"
#include "spritecomponent.h"
#include "texture.h"

class Game
{
public:
    Game();
    bool initialize();
    void runLoop();
    bool shutDown();

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    void addSprite(SpriteComponent* sprite);
    void removeSprite(SpriteComponent* sprite);

    SDL_Renderer* getRenderer(){return renderer;}

private:
    static const int WIN_WIDTH = 1024;
    static const int WIN_HEIGHT = 720;

    void processInput();
    void updateGame();
    void generateOutput();

    void loadData();
    void unloadData();

    bool loadShaders();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext gl_context;

    Uint32 mTicksCount;
    float mDeltaTime;

    std::vector<Actor*> mActors;
    std::vector<Actor*> mPendingActors;
    bool mUpdatingActors;

    Shader* mSpriteShader;

    std::vector<SpriteComponent*> mSprites;

};

#endif