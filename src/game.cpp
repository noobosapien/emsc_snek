#include "headers/gamepch.h"

Game:: Game(): mUpdatingActors(false), mSnake(nullptr)
{
}

int Game::WIN_WIDTH = 800;
int Game::WIN_HEIGHT = 800;
glm::vec2 Game::WIN_RES = glm::vec2(1.0);

bool Game::initialize(){
    Engine::initialize(Game::WIN_WIDTH, Game::WIN_HEIGHT);
    
    if(!loadShaders()){
        printf("Failed to load shaders\n");
        return false;
    }

    loadData();
    loadNetwork();

    // mGameState = EStart;

    mGameState = EStart;

    return true;
}

void Game::runLoop(){
    processInput();
    updateGame();
    generateOutput();
}

bool Game::shutDown(){
    Engine::shutDown();
    
    mCircleShader->unLoad();
    delete mCircleShader;

    mSpriteShader->unLoad();
    delete mSpriteShader;

    //delete actors
    while(!mActors.empty()){
        delete mActors.back();
    }

    //delete ui
    return true;

}

void Game::setWinDim(int width, int height){
    printf("w(%d) h(%d)\n", width, height);
    // height = 2000;
    // width = 2000;
    if(height > width){
        Game::WIN_HEIGHT = height;
        Game::WIN_WIDTH = width;
        Game::WIN_RES = glm::vec2(1.0, (float)height/width);

        SDL_SetWindowSize(window, width, height);
        glViewport(-height/2 + width/2, 0, height, height);
    }else{
        Game::WIN_HEIGHT = height;
        Game::WIN_WIDTH = width;
        Game::WIN_RES = glm::vec2((float)width/height, 1.0);

        SDL_SetWindowSize(window, width, height);
        // glViewport(0, -width/2 + height/2, width, width);
        glViewport(0, -width/2 + height/2, width, width);

    }
}

void Game::setInput(char* input) {
    printf("%s\n", input);

    std::string in = std::string(input);
    if(in == "pause"){
        setState(Game::EPause);
    }
    else if (in == "play"){
        setState(Game::EGameplay);
    }
    else if (in == "start"){
        setState(EGameplay);
    }
    else if(in == "end"){
        resetGame();
    }
    else{
        jsInput.push_back(in);
    }
}

void Game::resetGame(){
    //unload data
    unloadData();
    //load data
    loadData();
    //EM_ASM to set hero header
    EM_ASM(backToStart());
    //set state to start
    setState(EStart);
}

//private

void Game::processInput(){
    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                mGameState = EStart;
                break;
            case SDL_KEYDOWN:
                if(!event.key.repeat){
                    if(mGameState == EGameplay || mGameState == EPause){ //might need to change this when the HUD is included
                        handleKeyPress(event.key.keysym.sym);
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(mGameState == EGameplay || mGameState == EPause){
                    handleKeyPress(event.button.button);
                }
                break;
            default:
                break;
        }
        
    }

    if(mGameState == EGameplay){
        mUpdatingActors = true;
        for(auto actor: mActors){
            if(actor->getState() == Actor::EActive){
                actor->processInput(event);
            }
        }
        mUpdatingActors = false;
    }
}

void Game::updateGame(){
    Engine::updateGame();

    if(mGameState == EGameplay){
        mWebSocket->processAllPackets();
        mWebSocket->sendOutgoing();

        //Actor stuff

        mUpdatingActors = true;
        for(auto actor: mActors){
            actor->update(this->mDeltaTime);
        }
        mUpdatingActors = false;

        for(auto pending: mPendingActors){
            pending->computeWorldTransform();
            mActors.emplace_back(pending);
        }
        mPendingActors.clear();

        std::vector<Actor*> deadActors;
        for(auto actor: mActors){
            if(actor->getState() == Actor::EDead){
                deadActors.emplace_back(actor);
            }
        }

        for(auto actor: deadActors){
            delete actor;
        }
    }

}

/*
    IMPORTANT FOR SPRITES
*/
void Game::generateOutput(){
    Engine::generateOutput();

    //set shaders, vertex arrays and draw the sprites

    for(auto border: mBorders){
        border->draw(mBorderShader);
    }

    for(auto sprite: mSprites){
        sprite->draw(mSpriteShader);
    }

    if(mDebug)
        for(auto circle: mCircles){
            circle->draw(mCircleShader);
        }

    SDL_GL_SwapWindow(window);
}

void Game::addActor(Actor* actor){
    if(mUpdatingActors){
        mPendingActors.emplace_back(actor);
    }else{
        mActors.emplace_back(actor);
    }
}

void Game::removeActor(Actor* actor){
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if(iter != mPendingActors.end()){

        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if(iter != mActors.end()){

        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::loadData(){
    mCamera = new Camera();
    Snake* snake = new Snake(this);
    
    //to add the border create an actor
    Actor* temp = new Actor(this);
    BorderComponent* border = new BorderComponent(temp, 100);
}

void Game::unloadData(){
    // while(!mActors.empty()){
    //    removeActor(mActors.back());
    // }

    if(mSnake){
        delete mSnake;
        mSnake = nullptr;
    }
    
    for(auto actor : mActors){
        removeActor(actor);
    }

    for(auto i : mTextures){
        i.second->unload();
        delete i.second;
    }
    mTextures.clear();
    delete mCamera;
}

void Game::loadNetwork(){
    WebsockClient::staticInit(this, "Test");
    mWebSocket = WebsockClient::sInstance;
}

/*
    Shaders refactor later
*/
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

bool Game::loadShaders(){

    if(!loadCircleShader())
        return false;
    
    if(!loadSpriteShader())
        return false;
    
    if(!loadBorderShader())
        return false;
    
    return true;
}


bool Game::loadCircleShader(){
    mCircleShader = new Shader();

    if(!mCircleShader->load("src/shaders/circle.vert", "src/shaders/circle.frag"))
        return false;
    
    mCircleShader->setActive();

    float vertices[] = {
        -1.f,  1.f, 0.f, 0.f, 0.3f, 0.f,
		 1.f,  1.f, 0.f, 0.f, 0.3f, 0.f,
		 1.f, -1.f, 0.f, 0.f, 0.3f, 0.f,
		-1.f, -1.f, 0.f, 0.f, 0.3f, 0.f
    };

    unsigned int indices[] = {
		0, 1, 2,
        2, 3, 0
	};

    mCircleShader->setVertexData(vertices, 4, indices, 6, 6);

    mCircleShader->setAttrib("a_position", 3, 6, 0);
    mCircleShader->setAttrib("a_color", 3, 6, 3);

    return true;
}

bool Game::loadSpriteShader(){
    mSpriteShader = new Shader();

    if(!mSpriteShader->load("src/shaders/sprite.vert", "src/shaders/sprite.frag")){
        return false;
    }

    mSpriteShader->setActive();

    float vertices[] = {
        -1.f,  1.f, 0.f, 0.f, 1.f,
		 1.f,  1.f, 0.f, 1.f, 1.f,
		 1.f, -1.f, 0.f, 1.f, 0.f,
		-1.f, -1.f, 0.f, 0.f, 0.f
    };

	unsigned int indices[] = {
		0, 1, 2,
        2, 3, 0
	};
    

    mSpriteShader->setVertexData(vertices, 4, indices, 6, 5);

    mSpriteShader->setAttrib("a_position", 3, 5, 0);
    mSpriteShader->setAttrib("a_texCoord", 2, 5, 3);
    
    return true;
}

bool Game::loadBorderShader(){
    mBorderShader = new Shader();

    if(!mBorderShader->load("src/shaders/border.vert", "src/shaders/border.frag")){
        return false;
    }

    mBorderShader->setActive();

    float vertices[] = {
        -3.f,  3.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		 3.f,  3.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
		 3.f, -3.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		-3.f, -3.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f
    };

	unsigned int indices[] = {
		0, 1, 2,
        2, 3, 0
	};
    

    mBorderShader->setVertexData(vertices, 4, indices, 6, 8);

    mBorderShader->setAttrib("a_position", 3, 8, 0);
    mBorderShader->setAttrib("a_vertCoord", 2, 8, 3);
    mBorderShader->setAttrib("a_color", 3, 8, 5);
    
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void Game::addSprite(SpriteComponent* sprite){
    int drawOrder = sprite->getDrawOrder();

    auto iter = mSprites.begin();

    for(; iter != mSprites.end(); ++iter){
        if(drawOrder < (*iter)->getDrawOrder())
            break;
    }
    mSprites.insert(iter, sprite);
}

void Game::removeSprite(SpriteComponent* sprite){
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);

    if(iter != mSprites.end()){
        mSprites.erase(iter);
    }
}

Texture* Game::getTexture(const std::string& filename){
    Texture* tex = nullptr;

    auto iter = mTextures.find(filename);

    if(iter != mTextures.end())
        tex = iter->second;
    else{
        tex = new Texture();

        if(tex->load(filename))
            mTextures.emplace(filename, tex);
        else{
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

void Game::addCircle(CircleComponent* circle){
    int drawOrder = circle->getDrawOrder();

    auto iter = mCircles.begin();

    for(; iter != mCircles.end(); ++iter){
        if(drawOrder < (*iter)->getDrawOrder())
            break;
    }

    mCircles.insert(iter, circle);
}

void Game::removeCircle(CircleComponent* circle){
    auto iter = std::find(mCircles.begin(), mCircles.end(), circle);
    mCircles.erase(iter);
}

void Game::addBorder(BorderComponent* border){
    int drawOrder = border->getDrawOrder();

    auto iter = mBorders.begin();

    for(; iter != mBorders.end(); ++iter){
        if(drawOrder < (*iter)->getDrawOrder())
            break;
    }

    mBorders.insert(iter, border);
}

void Game::removeBorder(BorderComponent* border){
    auto iter = std::find(mBorders.begin(), mBorders.end(), border);
    mBorders.erase(iter);
}

void Game::setSnake(Snake* snake){
    mSnake = snake;
}

Snake* Game::getSnake(){
    return mSnake;
}

void Game::setState(GameState state){
    mGameState = state;
}

Game::GameState Game::getState(){
    return mGameState;
}

void Game::handleKeyPress(int key){
    switch(key){
        case SDLK_p:
            if(mGameState == EGameplay){
                setState(Game::EPause);
                // SDL_SetRelativeMouseMode(SDL_FALSE);
            }else {
                setState(Game::EGameplay);
            }
            
            break;
        default:
            break;
    }
}


std::deque<std::string>& Game::getJSInput(){
    return jsInput;
}