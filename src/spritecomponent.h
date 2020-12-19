#include "component.h"
#include "texture.h"

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

class SpriteComponent:Component{

public:
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void draw(class Shader* shader);
    virtual void setTexture(class Texture* texture);

    int getDrawOrder(){return mDrawOrder;}
    int getTexWidth(){return mTexWidth;}
    int getTexHeight(){return mTexHeight;}

private:
    class Texture* mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;

};

#endif