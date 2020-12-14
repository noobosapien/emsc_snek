#include "actor.h"
#include "game.h"
#include "component.h"

Actor::Actor(Game* game): mState(EActive),
mPosition(Vector2()),
mScale(1.0f),
mRotation(0.0f),
mGame(game),
mRecomputeWorldTransform(true)
{
    mGame->addActor(this);
}

Actor::~Actor(){
    mGame->removeActor(this);
    
    for(auto component: mComponents){
        delete component;
    }
}

void Actor::update(float delta){
    if(mState == EActive){
        computeWorldTransform();
        updateComponents(delta);
        updateActor(delta);
        computeWorldTransform();
    }
}

void Actor::updateComponents(float delta){
    for(auto component: mComponents){
        component->update(delta);
    }
}

void Actor::updateActor(float delta){}

void Actor::processInput(SDL_Event& event){
    if(mState == EActive){

        for(auto component: mComponents){
            component->processInput(event);
        }

        actorInput(event);
    }
}

void Actor::actorInput(const SDL_Event& event){}

void Actor::computeWorldTransform(){
    if(mRecomputeWorldTransform){
        mRecomputeWorldTransform = false;

        //todo scale a matrix4 then rotate then translate

        for(auto component: mComponents){
            component->onUpdateWorldTransform();
        }
    }
}

void Actor::addComponent(class Component* component){
    
    int order = component->getUpdateOrder();

    auto iter = mComponents.begin();

    for(; iter != mComponents.end(); ++iter){
        if(order < (*iter)->getUpdateOrder()){
            break;
        }
    }

    mComponents.insert(iter, component);
}

void Actor::removeComponent(class Component* component){
    
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);

    if(iter != mComponents.end()){
        mComponents.erase(iter);
    }
}