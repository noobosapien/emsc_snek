#include "circlecomponent.h"

CircleComponent::CircleComponent(Actor* owner, int drawOrder): Component(owner), mRadius(0.f), mDrawOrder(drawOrder){
    mOwner->getGame()->addCircle(this);
}

CircleComponent::~CircleComponent(){}

const glm::vec2& CircleComponent::getCenter() const{
    return mOwner->getPosition();
}

float CircleComponent::getRadius() const{
    // return mOwner->getScale() * mRadius;
    return mRadius;
}


void CircleComponent::draw(Shader* shader){
    shader->setActive();

    glm::mat4 model = glm::mat4(1.f);
    glm::vec2 resolution = glm::vec2(Game::WIN_WIDTH, Game::WIN_HEIGHT);

    float thickness = 5.f;

    model = mOwner->getWorldTransform();

    // projection = glm::perspective(glm::radians(45.0f), ((float)Game::WIN_WIDTH / (float)Game::WIN_HEIGHT), 0.1f, 100.0f);

    shader->setMatrixUniform("u_model", model);
    shader->setMatrixUniform("u_viewproj", mOwner->getGame()->getCamera()->getViewProj());

    shader->setVec2Uniform("u_resolution", resolution);
    shader->setFloatUniform("u_radius", mRadius);
    shader->setVec2Uniform("u_mid", mOwner->getPosition());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Intersect(const CircleComponent* a, const CircleComponent* b){
    
    glm::vec2 diff = glm::vec2(1024 * (b->getCenter().x - a->getCenter().x),
    720 * (b->getCenter().y - a->getCenter().y));

    float distSQ = (diff.x * diff.x) + (diff.y * diff.y);

    float radiiSQ = a->getRadius() + b->getRadius();
    radiiSQ *= radiiSQ;

    return distSQ <= radiiSQ;
}
