#include "headers/gamepch.h"

Game:: Game(): mUpdatingActors(false)
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

    //temp
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Error creating the framebuffer: %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, WIN_WIDTH, WIN_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Error creating the framebuffer %d \n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    // height = 1000;
    // width = 1000;
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
                    if(mGameState == EGameplay){ //might need to change this when the HUD is included
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
            case SDL_FINGERDOWN:
                if(mGameState == EGameplay){
                    new PauseMenu(this);
                }else if(!mUIStack.empty()){

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

    for( auto ui : mUIStack){
        if(ui->getState() == UIScreen::EActive){
            ui->update(this->mDeltaTime);
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
    Engine::generateOutput();

    //set shaders, vertex arrays and draw the sprites

    //temp
    // glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    // glEnable(GL_DEPTH_TEST);

    // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto ui: mUIStack){
        ui->draw(mTextShader, mUIShader);
    }
    

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

    //temp

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glDisable(GL_DEPTH_TEST);
    // // glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    // // glClear(GL_COLOR_BUFFER_BIT);

    // // mScreenQuad->setActive();
    // glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	// // glDrawArrays(GL_TRIANGLES, 0, 6);


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
    
    loadText("file");

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
    
    if(!loadTextShader())
        return false;
    
    if(!loadUIShader())
        return false;
    
    //temp
    mScreenQuad = new Shader();

    if(!mScreenQuad->load("src/shaders/screen.vert", "src/shaders/screen.frag")){
        return false;
    }

    mScreenQuad->setActive();

    float vertices[] = {
        -1.f,  1.f, 0.f, 1.f,
		 1.f,  1.f, 1.f, 1.f,
		 1.f, -1.f, 1.f, 0.f,
		-1.f, -1.f, 0.f, 0.f
    };

	unsigned int indices[] = {
		0, 1, 2,
        2, 3, 0
	};
    

    mScreenQuad->setVertexData(vertices, 4, indices, 6, 4);

    mScreenQuad->setAttrib("a_position", 2, 4, 0);
    mScreenQuad->setAttrib("a_texCoord", 2, 4, 2);
    
    return true;
}

bool Game::loadUIShader(){
    mUIShader = new Shader();

    if(!mUIShader->load("src/shaders/uitexture.vert", "src/shaders/uitexture.frag")){
        return false;
    }

    mUIShader->setActive();

    float vertices[] = {
        -1.f,  1.f, 0.f, 1.f,
		 1.f,  1.f, 1.f, 1.f,
		 1.f, -1.f, 1.f, 0.f,
		-1.f, -1.f, 0.f, 0.f
    };

	unsigned int indices[] = {
		0, 1, 2,
        2, 3, 0
	};
    

    mUIShader->setVertexData(vertices, 4, indices, 6, 4);

    mUIShader->setAttrib("a_vertex", 2, 4, 0);
    mUIShader->setAttrib("a_texCoord", 2, 4, 2);
    
    return true;
}

bool Game::loadTextShader(){
    mTextShader = new Shader();

    if(!mTextShader->load("src/shaders/text.vert", "src/shaders/text.frag"))
        return false;
    
    mTextShader->setActive();

	unsigned int indices[] = {
		2, 3, 0,
        0, 1, 2  
	};
    

    mTextShader->setVertexData(nullptr, 4, indices, 6, 4, GL_DYNAMIC_DRAW);

    mTextShader->setAttrib("a_vertex", 2, 4, 0);
    mTextShader->setAttrib("a_texCoord", 2, 4, 2);

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

        if(font->loadCharacters(fileName, 48)){
            mFonts.emplace(fileName, font);
        }else{
            delete font;
            font = nullptr;
        }

        return font;
    }
}

void Game::loadText(const std::string& fileName){
    // mText.clear();
    // std::ifstream file(fileName);
    // if(!file.is_open()){
    //     printf("Text file %s not found", fileName.c_str());
    //     return;
    // }

    // std::stringstream fileStream;
    // fileStream << file.rdbuf();
    // std::string contents = fileStream.str();

    //CONTINUE LATER!!!!!
    mText.emplace("Paused", "Paused");
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
        case SDLK_p:
            new PauseMenu(this);
            break;
        default:
            break;
    }
}

FT_Library& Game::getFtLib(){
    return mFtLib;
}