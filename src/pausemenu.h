#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "game.h"
#include "uiscreen.h"
#include <SDL2/SDL.h>

class PauseMenu: public UIScreen{
    public:
        PauseMenu(class Game* game);
        ~PauseMenu();

        void handleKeyPress(int key) override;
};

#endif