#include "food.h"

Food::Food(Game* game): Actor(game){

    SpriteComponent* sc = new SpriteComponent(this, 150);
    sc->setTexture(game->getTexture("src/textures/b.png"));


    mCircle = new CircleComponent(this, 150);
    mCircle->setRadius(60.f);
    
    mSnake = getGame()->getSnake();
}

Food::~Food(){

}

void Food::read(InputStream& inStream){
    
    uint32_t foodAction;

    while(inStream.getRemainingBitCount() > 8){
        inStream.read(foodAction);
        printf("foodAction %u\n", foodAction);
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
    
    uint32_t multiple;
    uint32_t value;

    inStream.read(multiple);
    inStream.read(value);

    float xVal = value / glm::pow(10, multiple);
    printf("pos x- %f\n", xVal);

    inStream.read(value);
    float yVal = value / glm::pow(10, multiple);
    printf("pos y- %f\n", yVal);

    this->setPosition(glm::vec2(xVal, yVal));

}

void Food::readAndSetScale(InputStream& inStream){
    //todo
    //read uint32 to float
    //get ratio and apply it to the actor
    uint32_t multiple;
    uint32_t value;

    inStream.read(multiple);
    inStream.read(value);

    float scale = value / glm::pow(10, multiple);
    printf("scale- %f\n", scale);

    this->setScale(scale);
}

void Food::updateActor(float delta){
    if(Intersect(mCircle, mSnake->getCircle())){
        OutputStream* outStream = new OutputStream();
        outStream->write(true);
        WebsockClient::sendMessage(*outStream);
    }
}
