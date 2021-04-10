#include "snake.h"

Snake::Snake(Game* game): Actor(game), mDirChanged(true){
    setScale(.01f);
    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->setTexture(game->getTexture("src/textures/a.png"));

    InputComponent* in = new InputComponent(this);
    in->setUpKey(SDLK_w);
    in->setDownKey(SDLK_s);
    in->setLeftKey(SDLK_a);
    in->setRightKey(SDLK_d);

    mCircle = new CircleComponent(this, 150);
    mCircle->setRadius(12.f);

    getGame()->setSnake(this);
    addBody();
    addBody();
}

Snake::~Snake(){}

void Snake::updateActor(float delta){
    if(mDirChanged){
        mDirChanged = false;

        for(auto body: mBodies){
            body->bodyDirChanged();
        }
    }

    for(auto body: mBodies){
        // printf("snake pos: %f %f  body pos: %f %f\n", getPosition().x, getPosition().y, body->getPosition().x, body->getPosition().y);
        if(Intersect(mCircle, body->getCircle())){
            printf("Colliding\n");
        }
    }

    getGame()->getCamera()->setPosition(glm::vec3(getPosition().x, getPosition().y, 0.f));
    // printf("snake pos: %f %f \n", getPosition().x, getPosition().y);
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

    Body* body = nullptr;

    if(!mBodies.empty()){
        body = new Body(getGame(), this, mBodies.back());
    }else{
        body = new Body(getGame(), this, nullptr);
    }

    if(body)
        mBodies.push_back(body);

    getGame()->getCamera()->addViewportVertices(-0.1, 0.1, -0.1, 0.1);
}

CircleComponent* Snake::getCircle(){
    return mCircle;
}