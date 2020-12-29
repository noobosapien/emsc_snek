#ifndef CIRCLECOMPONENT_H
#define CIRCLECOMPONENT_H

#include "component.h"
#include "actor.h"
#include "shader.h"
#include "game.h"
#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class CircleComponent: Component{
public:
    CircleComponent(Actor* owner, int drawOrder = 100);
    ~CircleComponent();

    void setRadius(float radius){mRadius = radius;}
    float getRadius() const;

    const glm::vec2& getCenter() const;

    void draw(Shader* shader);

    int getDrawOrder() {return mDrawOrder;}

private:
    float mRadius;
    int mDrawOrder;
};

bool Intersect(const CircleComponent* a, const CircleComponent* b);

#endif