#include "headers/gamepch.h"

Body::Body(Game* game, Snake* snake, Body* body): Actor(game), mSnake(snake), mPrevBody(body){
    setScale(.01f);

    SpriteComponent* sc = new SpriteComponent(this, 100);
    sc->setTexture(game->getTexture("src/textures/b.png"));

    mCircle = new CircleComponent(this, 150);
    mCircle->setRadius(12.0f);

    if(mPrevBody != nullptr){
        glm::vec2 newPos = mPrevBody->getNewBodyPos();
        setPosition(newPos);
    }else{
        glm::vec2 newPos = mSnake->getNewBodyPos();
        setPosition(newPos);
    }

    mFollow = new FollowComponent(this);
}

Body::~Body(){}

void Body::updateActor(float delta){

}

glm::vec2 Body::getNewBodyPos(){

    glm::vec2 forward = getForward();

    if(forward.x < -0.1f){
        return glm::vec2(getPosition().x + 0.1, getPosition().y);
    } else if(forward.x > 0.1f){
        return glm::vec2(getPosition().x - 0.1, getPosition().y);
    }

    if(forward.y < -0.1f){
        return glm::vec2(getPosition().x, getPosition().y + 0.1);
    } else if(forward.y > 0.1f){
        return glm::vec2(getPosition().x, getPosition().y - 0.1);
    }

    return glm::vec2();
}

void Body::bodyDirChanged(){
    mFollow->followDirChanged();
}