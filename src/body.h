#ifndef BODY_H
#define BODY_H

#include "actor.h"
#include "snake.h"
#include "circlecomponent.h"
#include "spritecomponent.h"
#include "followcomponent.h"

#include "glm/glm.hpp"

class Body: public Actor{

public:
    Body(Game* game, class Snake* snake, class Body* body = nullptr);
    ~Body();

    void updateActor(float delta) override;

    glm::vec2 getNewBodyPos();

    class Body* getPrevBody(){return mPrevBody;}
    class Snake* getSnake(){return mSnake;}

    void bodyDirChanged();

private:
    class CircleComponent* mCircle;
    class Snake* mSnake;
    class Body* mPrevBody;
    class FollowComponent* mFollow;
};

#endif