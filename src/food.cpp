#include "food.h"

Food::Food(Game* game): Actor(game){
    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->setTexture(game->getTexture("src/textures/food.png"));
}

Food::~Food(){

}

void Food::read(InputStream& inStream){
    //todo
}