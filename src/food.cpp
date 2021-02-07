#include "food.h"

Food::Food(Game* game): Actor(game){
    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->setTexture(game->getTexture("src/textures/b.png"));
}

Food::~Food(){

}

void Food::read(InputStream& inStream){
    
    uint32_t foodAction;

    while(inStream.getRemainingBitCount() > 8){
        inStream.read(foodAction);

        if(foodAction == FA_END)
            break;

        switch(foodAction){
            case FA_SETPOS:
                readAndSetPos(inStream);
                break;
            
            case FA_SETSCALE:
                readAndSetScale(inStream);
                break;

            default:
                break;
    }
    }
    
}

void Food::readAndSetPos(InputStream& inStream){
    //todo
    //create 2 floats for x and y read them from uint32
    //setposition for x and y
}

void Food::readAndSetScale(InputStream& inStream){
    //todo
    //read uint32 to float
    //get ratio and apply it to the actor
}