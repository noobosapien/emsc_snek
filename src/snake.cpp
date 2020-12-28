#include "snake.h"

Snake::Snake(Game* game): Actor(game), mDirChanged(true){
    setScale(.03f);
    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->setTexture(game->getTexture("src/textures/a.png"));

    InputComponent* in = new InputComponent(this);
    in->setUpKey(SDLK_w);
    in->setDownKey(SDLK_s);
    in->setLeftKey(SDLK_a);
    in->setRightKey(SDLK_d);

    mCircle = new CircleComponent(this, 150);
    mCircle->setRadius(12.f);

    Body* mBody = new Body(game, this);
    mBodies.push_back(mBody);

    Body*mBody2 = new Body(game, this, mBodies.back());
    mBodies.push_back(mBody2);

    mBody2 = new Body(game, this, mBodies.back());
    mBodies.push_back(mBody2);
    mBody2 = new Body(game, this, mBodies.back());
    mBodies.push_back(mBody2);
    mBody2 = new Body(game, this, mBodies.back());
    mBodies.push_back(mBody2);
}

Snake::~Snake(){}

void Snake::updateActor(float delta){
    if(mDirChanged){
        mDirChanged = false;

        for(auto body: mBodies){
            body->bodyDirChanged();
        }
    }
}


glm::vec2 Snake::getNewBodyPos(){

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

void Snake::addBody(){

    Body* body;

    if(!mBodies.empty()){
        body = new Body(getGame(), this, mBodies.back());
    }else{
        body = new Body(getGame(), this, nullptr);
    }

    // mBodies.push_back(body);
}