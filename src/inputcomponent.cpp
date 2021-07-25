#include "headers/gamepch.h"

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


    //js input
    if(!getOwner()->getGame()->getJSInput().empty()){
        std::string input = getOwner()->getGame()->getJSInput().front();
        getOwner()->getGame()->getJSInput().pop_front();
        printf("%s\n", input.c_str());

        //I know this is terrible

        if(input == "left"){
            if(getDirection() != RIGHT){
                setDirection(LEFT);
                getOwner()->snakeDirChanged();
            }
        }
        else if( input == "right"){
            if(getDirection() != LEFT){
                setDirection(RIGHT);
                getOwner()->snakeDirChanged();
            }
        }
        else if( input == "up"){
            if(getDirection() != DOWN){
                setDirection(UP);
                getOwner()->snakeDirChanged();
            }
        }
        else if( input == "down"){
            if(getDirection() != UP){
                setDirection(DOWN);
                getOwner()->snakeDirChanged();
            }
        }
    }
    
}