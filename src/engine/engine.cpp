#include "../headers/engine/enginepch.h"


Engine::Engine(): mTicksCount(0), mDeltaTime(0.0f), mDebug(false){

}

Engine::~Engine(){

}

void Engine::initialize(int width, int height){
    
    SDL_Init(SDL_INIT_VIDEO);

    // Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    
    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::shutDown(){
    SDL_Quit();
}

void Engine::updateGame(){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)); //60fps

    mDeltaTime = static_cast<float>(SDL_GetTicks() - mTicksCount) / 1000;
    
    if(mDeltaTime > 0.05f)
        mDeltaTime = 0.05f;
    
    mTicksCount = SDL_GetTicks();
}

void Engine::generateOutput(){
    glClearColor(0.130, 0.130, 0.130, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
