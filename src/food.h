#ifndef FOOD_H
#define FOOD_H

#include "actor.h"
#include "game.h"
#include "spritecomponent.h"
#include "network/inoutstreams.h"

class Food: public Actor{

public:

    enum FoodAction{
        FA_END,
        FA_SETPOS,
        FA_SETSCALE
    };

    Food(Game* game);
    ~Food();

    void read(InputStream& inStream);

    void readAndSetPos(InputStream& inStream);
    void readAndSetScale(InputStream& inStream);
};

#endif