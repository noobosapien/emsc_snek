#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <math.h>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_opengl.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GLES2/gl2.h>
#include <GL/glfw.h>

#include "actor.h"
#include "shader.h"
#include "spritecomponent.h"
#include "texture.h"
#include "movecomponent.h"
#include "inputcomponent.h"
#include "snake.h"
#include "circlecomponent.h"
#include "camera.h"


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

    void addCircle(CircleComponent* circle);
    void removeCircle(CircleComponent* circle);

    SDL_Renderer* getRenderer(){return renderer;}

    Texture* getTexture(const std::string& filename);

    void setDebug(bool value) {mDebug = value;}

    class Camera* getCamera(){return mCamera;}

    static const int WIN_WIDTH = 800;
    static const int WIN_HEIGHT = 800;

private:
    void processInput();
    void updateGame();
    void generateOutput();

    void loadData();
    void unloadData();

    bool loadShaders();
    bool loadSpriteShader();
    bool loadCircleShader();

    bool mDebug;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext gl_context;

    Uint32 mTicksCount;
    float mDeltaTime;

    std::vector<Actor*> mActors;
    std::vector<Actor*> mPendingActors;
    bool mUpdatingActors;

    Shader* mSpriteShader;
    Shader* mCircleShader;

    std::vector<SpriteComponent*> mSprites;

    std::unordered_map<std::string, Texture*> mTextures;

    std::vector<CircleComponent*> mCircles;

    class Camera* mCamera;

};

#endif