#include "followcomponent.h"

FollowComponent::FollowComponent(Actor* owner, int updateOrder): Component(owner, updateOrder), t(0.f){

}

FollowComponent::~FollowComponent(){}

void FollowComponent::update(float delta){

    glm::vec2 forward;
    glm::vec2 newPos;
    glm::vec2 ownerPos;

    if(reinterpret_cast<class Body*>(mOwner)->getPrevBody() != nullptr){
        forward = reinterpret_cast<class Body*>(mOwner)->getPrevBody()->getForward();
        ownerPos = reinterpret_cast<class Body*>(mOwner)->getPrevBody()->getPosition();
    }else{
        forward = reinterpret_cast<class Body*>(mOwner)->getSnake()->getForward();
        ownerPos = reinterpret_cast<class Body*>(mOwner)->getSnake()->getPosition(); 
    }

    if(forward.y < -0.1f){
        newPos = glm::vec2(ownerPos.x, ownerPos.y + 0.08f);
        mOwner->setRotation(-90.f);
    }else if(forward.y > 0.1f){
        newPos = glm::vec2(ownerPos.x, ownerPos.y - 0.08f);
        mOwner->setRotation(90.f);
    }

    if(forward.x < -0.1f){
        newPos = glm::vec2(ownerPos.x + 0.08f, ownerPos.y);
        mOwner->setRotation(180.f);
    }else if(forward.x > 0.1f){
        newPos = glm::vec2(ownerPos.x - 0.08f, ownerPos.y);
        mOwner->setRotation(0.f);
    }

    //interpolate to new position
    t >= 1.f ? t = 1.f : t += delta * 0.1;
    glm::vec2 pos = interpolate(mOwner->getPosition(), newPos, t);
    mOwner->setPosition(pos);
}

glm::vec2 FollowComponent::interpolate(const glm::vec2& a, const glm::vec2& b, float t){
    glm::vec2 res = a;

    res.x += (t * (b.x - a.x));
    res.y += (t * (b.y - a.y));

    return res;
}