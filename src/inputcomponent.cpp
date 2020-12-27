#include "inputcomponent.h"

InputComponent::InputComponent(Actor* owner): MoveComponent(owner),
mUpKey(0), mDownKey(0), mLeftKey(0), mRightKey(0){

}

InputComponent::~InputComponent(){

}

void InputComponent::processInput(SDL_Event& event){
    switch(event.type){
        case SDL_KEYDOWN:

        int key = event.key.keysym.sym;

        if(key == mUpKey)
            if(getDirection() != DOWN)
                setDirection(UP);
        
        if(key == mDownKey)
            if(getDirection() != UP)
                setDirection(DOWN);
        
        if(key == mLeftKey)
            if(getDirection() != RIGHT)
                setDirection(LEFT);

        if(key == mRightKey)
            if(getDirection() != LEFT)
                setDirection(RIGHT);
        
        // if(key == SDLK_SPACE)
        //     getOwner()->addBody();
    }
}