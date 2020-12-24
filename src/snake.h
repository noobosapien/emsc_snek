#ifndef SNAKE_H
#define SNAKE_H

#include "actor.h"
#include "game.h"
#include "spritecomponent.h"
#include "inputcomponent.h"
#include "circlecomponent.h"

#include <SDL2/SDL.h>

class Snake:Actor{
public:
    Snake(Game* game);
    ~Snake();

    void updateActor(float delta) override;

private:
    class CircleComponent* mCircle;
};

#endif