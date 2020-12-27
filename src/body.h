#include "actor.h"
#include "snake.h"
#include "circlecomponent.h"
#include "spritecomponent.h"

#include "glm/glm.hpp"

#ifndef BODY_H
#define BODY_H

class Body:Actor{

public:
    Body(Game* game, class Snake* snake, class Body* body = nullptr);
    ~Body();

    void updateActor(float delta) override;

    glm::vec2 getNewBodyPos();

private:
    class CircleComponent* mCircle;
    class Snake* mSnake;
    class Body* mPrevBody;
};

#endif