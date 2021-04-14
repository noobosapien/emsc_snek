#ifndef FOLLOWCOMPONENT_H
#define FOLLOWCOMPONENT_H

class FollowComponent: public Component{
public:
    FollowComponent(Actor* owner, int updateOrder = 10);
    ~FollowComponent();

    virtual void update(float delta) override;
    glm::vec2 interpolate(const glm::vec2& a, const glm::vec2& b, float delta);

    void followDirChanged(){t = 0.05f;}

private:
    float t;
};

#endif