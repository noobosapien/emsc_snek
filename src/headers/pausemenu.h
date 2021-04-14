#ifndef PAUSEMENU_H
#define PAUSEMENU_H

class PauseMenu: public UIScreen{
    public:
        PauseMenu(class Game* game);
        ~PauseMenu();

        void handleKeyPress(int key) override;
};

#endif