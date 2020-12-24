#include "snake.h"

Snake::Snake(Game* game): Actor(game){
    setScale(0.06f);
    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->setTexture(game->getTexture("src/textures/a.png"));

    InputComponent* in = new InputComponent(this);
    in->setUpKey(SDLK_w);
    in->setDownKey(SDLK_s);
    in->setLeftKey(SDLK_a);
    in->setRightKey(SDLK_d);

    mCircle = new CircleComponent(this, 150);
    mCircle->setRadius(12.f);
}

Snake::~Snake(){}

void Snake::updateActor(float delta){

}