#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <GLES2/gl2.h>
#include <GL/glfw.h>

#include "game.h"
#include "spritecomponent.h"
#include "actor.h"
#include "shader.h"
#include "math.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder):Component(owner),
mTexture(nullptr),
mDrawOrder(drawOrder),
mTexWidth(0),
mTexHeight(0){
    mOwner->getGame()->addSprite(this);
}

SpriteComponent::~SpriteComponent(){
    mOwner->getGame()->removeSprite(this);
}

void SpriteComponent::draw(Shader* shader){
    if(mTexture){
        Matrix4 scaleMat = Matrix4::createScale(
            static_cast<float>(mTexWidth), static_cast<float>(mTexHeight), 1.0f
        );

        Matrix4 world = scaleMat * mOwner->getWorldTransform();

        shader->setMatrixUniform("u_worldTransform", world);

        mTexture->setActive();
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SpriteComponent::setTexture(class Texture* texture){
    mTexture = texture;

    mTexWidth = texture->getWidth();
    mTexHeight = texture->getHeight();
}