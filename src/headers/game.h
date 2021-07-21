#ifndef GAME_H
#define GAME_H

class Game : public Engine
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

    void addBorder( BorderComponent* border);
    void removeBorder( BorderComponent* border);

    void setSnake( Snake* snake);
    Snake* getSnake();

    Texture* getTexture(const std::string& filename);

    Camera* getCamera(){return mCamera;}

    std::vector< UIScreen*>& getUIStack();
    void pushUI( UIScreen* screen);

    Font* getFont(const std::string& fileName);
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
    void updateGame() override;
    void generateOutput() override;

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

    std::vector<Actor*> mActors;
    std::vector<Actor*> mPendingActors;
    bool mUpdatingActors;

    GameState mGameState;
    Snake* mSnake;

    Shader* mSpriteShader;
    Shader* mCircleShader;
    Shader* mBorderShader;
    Shader* mTextShader;
    Shader* mUIShader;

    std::vector<SpriteComponent*> mSprites;

    std::unordered_map<std::string, Texture*> mTextures;

    std::vector<CircleComponent*> mCircles;

    std::vector< BorderComponent*> mBorders;

    std::vector< UIScreen*> mUIStack;

    std::unordered_map<std::string,  Font*> mFonts;

    std::unordered_map<std::string, std::string> mText;

    Camera* mCamera;

    WebsockClient* mWebSocket;

    //temp
    unsigned int mFBO;
    unsigned int textureColorbuffer;
    unsigned int rbo;
    Shader* mScreenQuad;

};

#endif