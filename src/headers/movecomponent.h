#ifndef MOVECOMP_H
#define MOVECOMP_H

#define PI 3.14159265359

class MoveComponent: public Component{
public:
    MoveComponent(Actor* owner, int updateOrder = 10);
    ~MoveComponent();

    void update(float delta) override;
    void setSpeed(int speed){mSpeed = speed;}
    int getSpeed(){return mSpeed;}

protected:
    enum DIRECTION{
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    void setDirection(DIRECTION dir){mDirection = dir;}
    DIRECTION getDirection(){return mDirection;}
    class Snake* getOwner(){return (class Snake*)mOwner;}

private:
    DIRECTION mDirection;
    glm::vec2 mVelocity;
    float mSpeed;
};

#endif