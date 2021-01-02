#include "bordercomponent.h"

BorderComponent::BorderComponent(Actor* owner, int drawOrder): Component(owner), mDrawOrder(drawOrder){
    mOwner->getGame()->addBorder(this);
}

BorderComponent::~BorderComponent(){}

void BorderComponent::draw(Shader* shader){
    
    shader->setActive();

    glm::mat4 model = glm::mat4(1.f);

    shader->setMatrixUniform("u_model", model);
    shader->setMatrixUniform("u_viewproj", mOwner->getGame()->getCamera()->getViewProj());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}