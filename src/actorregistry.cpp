#include "headers/gamepch.h"

ActorRegistry* ActorRegistry::sInstance = nullptr;

ActorRegistry::ActorRegistry(){
    
}

ActorRegistry::~ActorRegistry(){

}

void ActorRegistry::staticInit(){
    if(sInstance)
        delete sInstance;
    sInstance = new ActorRegistry();
}

void ActorRegistry::registerCreationFunc(uint32_t fourCCName, ActorCreationFunc creationFunc){
    mNameToActorCreationFunc[fourCCName] = creationFunc;
}


Actor* ActorRegistry::createActorObject(uint32_t fourCCName){
    ActorCreationFunc creationFunc = mNameToActorCreationFunc[fourCCName];
    Actor* actor = creationFunc();
    return actor;
}