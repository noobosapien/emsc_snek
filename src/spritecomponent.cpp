#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <GLES2/gl2.h>
#include <GL/glfw.h>
#include <iostream>

#include "game.h"
#include "spritecomponent.h"
#include "actor.h"
#include "shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
        shader->setActive();
        
        glm::mat4 model = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 projection = glm::mat4(1.f);

        model = mOwner->getWorldTransform();
        
        view = glm::translate(view, glm::vec3(0.f, 0.f, -1.f));

        projection = glm::perspective(glm::radians(45.0f), 1024.f / 720.f, 0.1f, 100.0f);

        shader->setMatrixUniform("u_model", model);
        shader->setMatrixUniform("u_view", view);
        shader->setMatrixUniform("u_projection", projection);
    
        mTexture->setActive();
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SpriteComponent::setTexture(class Texture* texture){
    mTexture = texture;

    mTexWidth = texture->getWidth();
    mTexHeight = texture->getHeight();
}