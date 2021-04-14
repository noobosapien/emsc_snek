#ifndef ANIMATEDSPRITECOMPONENT_H
#define ANIMATEDSPRITECOMPONENT_H

class AnimatedSpriteComponent: public SpriteComponent{

public:
    AnimatedSpriteComponent() = delete;
    AnimatedSpriteComponent(Actor* owner, int drawOrder = 100);
    virtual ~AnimatedSpriteComponent();

    void update(float delta) override;
    void addAnimTextures(const std::vector<Texture*>& textures);

    float getAnimFps(){return mAnimFps;}
    void setAnimFps(float fps){mAnimFps = fps;}

private:
    std::vector<std::vector<Texture*>> mAnimSprites;
    int mCurrAnim;
    float mCurrFrame;
    float mAnimFps;

};

#endif