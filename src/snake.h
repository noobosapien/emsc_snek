#ifndef SNAKE_H
#define SNAKE_H

#include "actor.h"
#include "game.h"
#include "spritecomponent.h"
#include "inputcomponent.h"
#include "circlecomponent.h"
#include "body.h"

#include <SDL2/SDL.h>

class Snake:Actor{
public:
    Snake(Game* game);
    ~Snake();

    void updateActor(float delta) override;

    glm::vec2 getNewBodyPos();
    void addBody();

private:
    class CircleComponent* mCircle;
    std::vector<class Body*> mBodies;
};

#endif