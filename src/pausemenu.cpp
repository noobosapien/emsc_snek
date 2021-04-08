#include "pausemenu.h"

PauseMenu::PauseMenu(Game* game): UIScreen(game){
    mGame->setState(Game::EPause);
    setRelativeMouseMode(false);
    setTitle("Paused", glm::vec3(1.0, 1.0, 1.0));

    addButton("Resume", [this](){
        close();
    });

    // addButton
}

PauseMenu::~PauseMenu(){
    setRelativeMouseMode(true);
    mGame->setState(Game::EGameplay);
}

void PauseMenu::handleKeyPress(int key){
    UIScreen::handleKeyPress(key);

    if(key == SDLK_p){
        close();
    }
}