#ifndef CIRCLECOMPONENT_H
#define CIRCLECOMPONENT_H

class CircleComponent: public Component{
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