#ifndef GAME_H
#define GAME_H

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

    FT_Library& getFtLib();

    static int WIN_WIDTH;
    static int WIN_HEIGHT;
    static glm::vec2 WIN_RES;

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
    bool loadTextShader();
    bool loadUIShader();

    bool mDebug;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext gl_context;

    FT_Library mFtLib;

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
    Shader* mTextShader;
    Shader* mUIShader;

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