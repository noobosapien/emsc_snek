#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <math.h>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> //use freetype
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
#include "bordercomponent.h"
#include "uiscreen.h"
#include "font.h"

#include "network/websockclient.h"


class Game
{
public:
    Game();
    bool initialize();
    void runLoop();
    bool shutDown();
    void setWinDim(int width, int height);

    enum GameState{
        EStart,
        EGameplay,
        EPause
    };

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    void addSprite(SpriteComponent* sprite);
    void removeSprite(SpriteComponent* sprite);

    void addCircle(CircleComponent* circle);
    void removeCircle(CircleComponent* circle);

    void addBorder(class BorderComponent* border);
    void removeBorder(class BorderComponent* border);

    void setSnake(class Snake* snake);
    class Snake* getSnake();

    SDL_Renderer* getRenderer(){return renderer;}

    Texture* getTexture(const std::string& filename);

    void setDebug(bool value) {mDebug = value;}

    class Camera* getCamera(){return mCamera;}

    std::vector<class UIScreen*>& getUIStack();
    void pushUI(class UIScreen* screen);

    class Font* getFont(const std::string& fileName);
    void loadText(const std::string& fileName);
    const std::string& getText(const std::string& key);

    void setState(GameState state);
    GameState getState();

    static int WIN_WIDTH;
    static int WIN_HEIGHT;

private:
    void processInput();
    void updateGame();
    void generateOutput();

    void handleKeyPress(int key);

    void loadData();
    void unloadData();
    void loadNetwork();

    //shaders
    bool loadShaders();
    bool loadSpriteShader();
    bool loadCircleShader();
    bool loadBorderShader();

    bool mDebug;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext gl_context;

    Uint32 mTicksCount;
    float mDeltaTime;

    std::vector<Actor*> mActors;
    std::vector<Actor*> mPendingActors;
    bool mUpdatingActors;

    GameState mGameState;
    class Snake* mSnake;

    Shader* mSpriteShader;
    Shader* mCircleShader;
    Shader* mBorderShader;

    std::vector<SpriteComponent*> mSprites;

    std::unordered_map<std::string, Texture*> mTextures;

    std::vector<CircleComponent*> mCircles;

    std::vector<class BorderComponent*> mBorders;

    std::vector<class UIScreen*> mUIStack;

    std::unordered_map<std::string, class Font*> mFonts;

    std::unordered_map<std::string, std::string> mText;

    class Camera* mCamera;

    class WebsockClient* mWebSocket;

};

#endif