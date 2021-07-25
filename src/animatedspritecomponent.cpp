#include "headers/gamepch.h"

AnimatedSpriteComponent::AnimatedSpriteComponent(Actor* owner, int drawOrder): SpriteComponent(owner, drawOrder),
mCurrAnim(0), mCurrFrame(0), mAnimFps(0)
{

}

AnimatedSpriteComponent::~AnimatedSpriteComponent(){

}

void AnimatedSpriteComponent::update(float delta){
    SpriteComponent::update(delta);

    if(mAnimSprites[mCurrAnim].size() > 0){
        mCurrFrame += mAnimFps * delta;

        if(mCurrFrame > mAnimSprites[mCurrAnim].size()){
            mCurrFrame -= mAnimSprites[mCurrAnim].size();
        }

        setTexture(mAnimSprites[mCurrAnim][static_cast<int>(mCurrFrame)]);
    }
}

void AnimatedSpriteComponent::addAnimTextures(const std::vector<Texture*>& textures){
    std::vector<Texture*> temp = textures;
    if(temp.size() > 0){
        mCurrFrame = 0.0f;
        setTexture(temp[0]);
    }
    mAnimSprites.push_back(temp);
}