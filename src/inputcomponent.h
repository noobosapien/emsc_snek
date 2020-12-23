#include "movecomponent.h"
#include <SDL2/SDL.h>

#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

class InputComponent: MoveComponent{
public:
    InputComponent(Actor* owner);
    ~InputComponent();

    void processInput(SDL_Event& event) override;

    void setUpKey(int key){mUpKey = key;}
    void setDownKey(int key){mDownKey = key;}
    void setLeftKey(int key){mLeftKey = key;}
    void setRightKey(int key){mRightKey = key;}

    int getUpKey(){return mUpKey;}
    int getDownKey(){return mDownKey;}
    int getLeftKey(){return mLeftKey;}
    int getRightKey(){return mRightKey;}

private:
    int mUpKey;
    int mDownKey;
    int mLeftKey;
    int mRightKey;
};

#endif