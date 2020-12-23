#include "movecomponent.h"
#include "game.h"
#include <stdio.h>

MoveComponent::MoveComponent(Actor* owner, int updateOrder): 
Component(owner, updateOrder),
mDirection(RIGHT),
mVelocity(glm::vec2(0.f, 0.f)),
mSpeed(0.4){

}

MoveComponent::~MoveComponent(){

}

void MoveComponent::update(float delta){
    switch(mDirection){
        case LEFT:
            mOwner->setRotation(180.f);
            mVelocity = glm::vec2(-1.f, 0.f);
            break;
        
        case RIGHT:
            mOwner->setRotation(0.f);
            mVelocity = glm::vec2(1.f, 0.f);
            break;
        
        case UP:
            mOwner->setRotation(90.f);
            mVelocity = glm::vec2(0.f, 1.f);
            break;
        
        case DOWN:
            mOwner->setRotation(-90.f);
            mVelocity = glm::vec2(0.f, -1.f);
            break;
        
        default:
            mOwner->setRotation(180.f);
            mVelocity = glm::vec2(1.f, 0.f);
            break;
    }

    glm::vec2 pos = mOwner->getPosition();

    pos += mVelocity * mSpeed * delta;

    //clamp
    if(pos.x > 0.5){
        pos.x = -0.5;
    }else if(pos.x < -0.5){
        pos.x = 0.5;
    }

    if(pos.y > 0.5){
        pos.y = -0.5;
    }else if(pos.y < -0.5){
        pos.y =0.5;
    }

    mOwner->setPosition(pos);
}