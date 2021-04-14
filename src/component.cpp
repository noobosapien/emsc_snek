#include "headers/gamepch.h"

Component::Component(Actor* owner, int updateOrder): mOwner(owner), mUpdateOrder(updateOrder)
{
    mOwner->addComponent(this);
}

Component::~Component()
{
    mOwner->removeComponent(this);
}

void Component::update(float delta){
    
}

void Component::processInput(SDL_Event& event){
    
}

void Component::onUpdateWorldTransform(){

}