#include "headers/gamepch.h"

InputComponent::InputComponent(Actor* owner): MoveComponent(owner),
mUpKey(0), mDownKey(0), mLeftKey(0), mRightKey(0){

}

InputComponent::~InputComponent(){

}

void InputComponent::processInput(SDL_Event& event){
    switch(event.type){
        case SDL_FINGERDOWN:
            // getOwner()->addBody();
            break;
        case SDL_KEYDOWN:

            int key = event.key.keysym.sym;

            if(key == mUpKey)
                if(getDirection() != DOWN){
                    setDirection(UP);
                    getOwner()->snakeDirChanged();
                }
                    
            if(key == mDownKey)
                if(getDirection() != UP){
                    setDirection(DOWN);
                    getOwner()->snakeDirChanged();
                }
                
            if(key == mLeftKey)
                if(getDirection() != RIGHT){
                    setDirection(LEFT);
                    getOwner()->snakeDirChanged();
                }
                    
            if(key == mRightKey)
                if(getDirection() != LEFT){
                    setDirection(RIGHT);
                    getOwner()->snakeDirChanged();
                }
                    
            if(key == SDLK_SPACE)
                getOwner()->addBody();

            break;
        
    }
}