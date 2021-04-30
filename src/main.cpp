#include "headers/gamepch.h"

static Game* game;

void loop(){
    game->runLoop();
}

extern "C"
EMSCRIPTEN_KEEPALIVE

void set_win_dim(int width, int height){
    game->setWinDim(width, height);
}

int main(int argc, char* argv[]){
    game = new Game;

    game->setDebug(false);
    // game->setDebug(true);

    bool success = game->initialize();

    if(success)
        emscripten_set_main_loop(loop, 0, 0);
    else
        delete game;
    
    return 0;
}