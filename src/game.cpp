#include "game.h"


 Game:: Game(): mTicksCount(0), mDeltaTime(0.0f), mUpdatingActors(false)
{
}

bool Game::initialize(){
    SDL_Init(SDL_INIT_VIDEO);

    // Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_CreateWindowAndRenderer(Game::WIN_WIDTH, Game::WIN_HEIGHT, 0, &window, &renderer);

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    loadData();

    return true;
}

void Game::runLoop(){
    processInput();
    updateGame();
    generateOutput();
}

bool Game::shutDown(){
    return true;
}

//private

void Game::processInput(){
    SDL_Event event;

    if(SDL_PollEvent(&event)){
        mUpdatingActors = true;
        for(auto actor: mActors){
            actor->processInput(event);
        }
        mUpdatingActors = false;
    }
}

void Game::updateGame(){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)); //60fps

    mDeltaTime = static_cast<float>(SDL_GetTicks() - mTicksCount) / 1000;
    
    if(mDeltaTime > 0.05f)
        mDeltaTime = 0.05f;
    
    mTicksCount = SDL_GetTicks();

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

void Game::generateOutput(){
    glClearColor(1, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //set shaders, vertex arrays and draw the sprites

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
    new Actor(this);
}

void Game::unloadData(){

}