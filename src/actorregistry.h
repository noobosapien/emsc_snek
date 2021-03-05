#ifndef ACTORREGISTRY_H
#define ACTORREGISTRY_H

#include <unordered_map>
#include <cstdint>
#include "actor.h"

typedef Actor* (*ActorCreationFunc)();

class ActorRegistry{
public:
    virtual ~ActorRegistry();
    static void staticInit();
    static ActorRegistry* sInstance;

    void registerCreationFunc(uint32_t fourCCName, ActorCreationFunc creationFunc);
    class Actor* createActorObject(uint32_t fourCCName);
private:
    ActorRegistry();
    std::unordered_map<uint32_t, ActorCreationFunc> mNameToActorCreationFunc;
    Game* mGame;
};

#endif