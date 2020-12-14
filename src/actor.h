#ifndef ACTOR_H
#define ACTOR_H

#include <vector>

#include "math.h"
#include <SDL2/SDL.h>

class Actor{
public:
    enum State{
        EActive,
        EPaused,
        EDead
    };

    Actor(class Game* game);
    virtual ~Actor();

    void update(float delta);
    void updateComponents(float delta);
    virtual void updateActor(float delta);

    void processInput(SDL_Event& event);
    virtual void actorInput(const SDL_Event& event);

    inline void setPosition(const Vector2& pos){mPosition = pos; mRecomputeWorldTransform = true;}
    inline const Vector2& getPosition(){return mPosition;}
    inline void setScale(float scale){mScale = scale; mRecomputeWorldTransform = true;}
    inline const float getScale(){return mScale;}
    inline void setRotation(float rotation){mRotation = rotation; mRecomputeWorldTransform = true;}
    inline const float getRotation(){return mRotation;}

    void computeWorldTransform();
    inline const Matrix4& getWorldTransform(){return mWorldTransform;}

    Vector2 getForward(){return Vector2(cos(mRotation), sin(mRotation));}

    State getState(){return mState;}
    void setState(State state){mState = state;}

    class Game* getGame(){return mGame;}

    void addComponent(class Component* component);
    void removeComponent(class Component* component);

private:
    State mState;
    Vector2 mPosition;
    Matrix4 mWorldTransform;
    float mScale;
    float mRotation;
    bool mRecomputeWorldTransform;

    std::vector<class Component*> mComponents;
    class Game* mGame;
};

#endif