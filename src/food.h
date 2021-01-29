#ifndef FOOD_H
#define FOOD_H

#include "actor.h"
#include "game.h"
#include "spritecomponent.h"
#include "network/inoutstreams.h"

class Food: public Actor{

public:
    Food(Game* game);
    ~Food();

    void read(InputStream& inStream);
};

#endif