#include "../headers/engine/enginepch.h"


Engine::Engine(): mTicksCount(0), mDeltaTime(0.0f){

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

    //Loads freetype
    if(FT_Init_FreeType(&mFtLib)){
        printf("ERROR:FREETYPE Could not initialize the library\n");
        return;
    }

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    
    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::shutDown(){
    //unload Freetype
    FT_Done_FreeType(mFtLib);

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
    glClearColor(0.313, 0.176, 0.47, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
