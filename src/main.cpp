#include <emscripten.h>
#include "game.h"

static Game* game;

void loop(){
    game->runLoop();
}

int main(int argc, char* argv[]){
    game = new Game;

    game->setDebug(true);
    bool success = game->initialize();

    if(success)
        emscripten_set_main_loop(loop, 0, 0);

    return 0;
}