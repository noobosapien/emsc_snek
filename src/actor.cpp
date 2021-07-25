#include "headers/gamepch.h"

Actor::Actor(Game* game): mState(EActive),
mPosition(glm::vec2(0.f, 0.f)),
mWorldTransform(glm::mat4(1.f)),
mScale(1.f),
mRotation(0.0f),
mGame(game),
mRecomputeWorldTransform(true)
{
    mGame->addActor(this);
}

Actor::~Actor(){
    for(auto component: mComponents){
        std::cout << "Component size: " << mComponents.size() << std::endl;
        delete component;
    }
    std::cout << "Components size: " << mComponents.size() << std::endl;

    mGame->removeActor(this);
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
        
        glm::mat4 trans = glm::mat4(1.f);

        trans = glm::translate(trans, glm::vec3(mPosition.x, mPosition.y, 0.f));
        trans = glm::rotate(trans, glm::radians(mRotation), glm::vec3(0.f, 0.f, 1.f));
        trans = glm::scale(trans, glm::vec3(mScale, mScale, mScale));

        mWorldTransform = trans;

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
    printf("removeComponents: %lu\n", mComponents.size());
    if(iter != mComponents.end()){
        mComponents.erase(iter);
    }
}