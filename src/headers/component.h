#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:
    Component(Actor* owner, int updateOrder = 100);
    virtual ~Component();

    virtual void update(float delta);
    virtual void processInput(SDL_Event& event);
    virtual void onUpdateWorldTransform();
    
    int getUpdateOrder(){return mUpdateOrder;}

protected:
    Actor* mOwner;
    int mUpdateOrder;
};

#endif