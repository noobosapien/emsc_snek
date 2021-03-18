#include "game.h"
#include "pausemenu.h"


 Game:: Game(): mTicksCount(0), mDeltaTime(0.0f), mUpdatingActors(false), mDebug(false)
{
}

int Game::WIN_WIDTH = 800;
int Game::WIN_HEIGHT = 800;

bool Game::initialize(){
    SDL_Init(SDL_INIT_VIDEO);

    // Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_CreateWindowAndRenderer(Game::WIN_WIDTH, Game::WIN_HEIGHT, 0, &window, &renderer);
    // SDL_CreateWindowAndRenderer(1920, 1920, 0, &window, &renderer);

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    if(!loadShaders()){
        printf("Failed to load shaders\n");
    }
    
    glViewport(0, 0, Game::WIN_WIDTH, Game::WIN_HEIGHT);
    // glViewport(0, 0, 1920, 1920);
    
    loadData();
    loadNetwork();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(TTF_Init() != 0){
        printf("Failed to initialize SDL_ttf");
        return false;
    }

    // mGameState = EStart;

    mGameState = EGameplay;

    return true;
}

void Game::runLoop(){
    processInput();
    updateGame();
    generateOutput();
}

bool Game::shutDown(){
    mCircleShader->unLoad();
    delete mCircleShader;

    mSpriteShader->unLoad();
    delete mSpriteShader;
    return true;

    //delete actors
    while(!mActors.empty()){
        delete mActors.back();
    }
}

void Game::setWinDim(int width, int height){

    if(height > width){
        Game::WIN_HEIGHT = height;
        Game::WIN_WIDTH = width;

        SDL_SetWindowSize(window, width, height);
        glViewport(-height/2 + width/2, 0, height, height);
    }else{
        Game::WIN_HEIGHT = height;
        Game::WIN_WIDTH = width;

        SDL_SetWindowSize(window, width, height);
        glViewport(0, -width/2 + height/2, width, width);
    }
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
                    if(mGameState == EGameplay){
                        handleKeyPress(event.key.keysym.sym);
                    }else if(!mUIStack.empty()){
                        mUIStack.back()->handleKeyPress(event.key.keysym.sym);
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(mGameState == EGameplay){
                    handleKeyPress(event.button.button);
                }else if(!mUIStack.empty()){
                    mUIStack.back()->handleKeyPress(event.button.button);
                }
                break;
            default:
                break;
        }

        if(mGameState == EGameplay){
            mUpdatingActors = true;
            for(auto actor: mActors){
                if(actor->getState() == Actor::EActive){
                    actor->processInput(event);
                }
            }
            mUpdatingActors = false;
        }else if(!mUIStack.empty()){
            mUIStack.back()->processInput(event);
        }
    }
}

void Game::updateGame(){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)); //60fps

    mWebSocket->processAllPackets();
    mWebSocket->sendOutgoing();

    mDeltaTime = static_cast<float>(SDL_GetTicks() - mTicksCount) / 1000;
    
    if(mDeltaTime > 0.05f)
        mDeltaTime = 0.05f;
    
    mTicksCount = SDL_GetTicks();

    if(mGameState == EGameplay){
        //Actor stuff

        mUpdatingActors = true;
        for(auto actor: mActors){
            actor->update(mDeltaTime);
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

    for( auto ui : mUIStack){
        if(ui->getState() == UIScreen::EActive){
            ui->update(mDeltaTime);
        }
    }

    auto iter = mUIStack.begin();
    while(iter != mUIStack.end()){
        if((*iter)->getState() == UIScreen::EClosing){
            delete *iter;
            iter = mUIStack.erase(iter);
        }else{
            ++iter;
        }
    }
}

/*
    IMPORTANT FOR SPRITES
*/
void Game::generateOutput(){
    glClearColor(0.313, 0.176, 0.47, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    
    //set shaders, vertex arrays and draw the sprites

    for(auto border: mBorders){
        border->draw(mBorderShader);
    }

    for(auto sprite: mSprites){
        sprite->draw(mSpriteShader);
    }

    
    for(auto ui: mUIStack){
        ui->draw(mSpriteShader);
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
    while(!mActors.empty()){
        delete mActors.back();
    }

    for(auto i : mTextures){
        i.second->unload();
        delete i.second;
    }
    mTextures.clear();
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

    mSprites.erase(iter);
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


std::vector<class UIScreen*>& Game::getUIStack(){
    return mUIStack;
}

void Game::pushUI(class UIScreen* screen){
    mUIStack.emplace_back(screen);
}

void Game::setState(GameState state){
    mGameState = state;
}

Game::GameState Game::getState(){
    return mGameState;
}

Font* Game::getFont(const std::string& fileName){
    auto iter = mFonts.find(fileName);

    if(iter != mFonts.end()){
        return iter->second;
    }else{
        Font* font = new Font(this);

        if(font->load(fileName)){
            mFonts.emplace(fileName, font);
        }else{
            font->unLoad();
            delete font;
            font = nullptr;
        }

        return font;
    }
}

void Game::loadText(const std::string& fileName){
    mText.clear();
    std::ifstream file(fileName);
    if(!file.is_open()){
        printf("Text file %s not found", fileName.c_str());
        return;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();

    //CONTINUE LATER!!!!!
}

const std::string& Game::getText(const std::string& key){
    static std::string errorMsg("KEY NOT FOUND");

    auto iter = mText.find(key);
    if(iter != mText.end()){
        return iter->second;
    }else{
        return errorMsg;
    }
}

void Game::handleKeyPress(int key){
    switch(key){
        case SDLK_ESCAPE:
            new PauseMenu(this);
            break;
        default:
            break;
    }
}