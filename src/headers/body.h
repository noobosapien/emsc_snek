#ifndef BODY_H
#define BODY_H

class Body: public Actor{

public:
    Body(Game* game, class Snake* snake, class Body* body = nullptr);
    ~Body();

    void updateActor(float delta) override;

    glm::vec2 getNewBodyPos();

    class Body* getPrevBody(){return mPrevBody;}
    class Snake* getSnake(){return mSnake;}

    void bodyDirChanged();

    class CircleComponent* getCircle(){return mCircle;}

private:
    class CircleComponent* mCircle;
    class Snake* mSnake;
    class Body* mPrevBody;
    class FollowComponent* mFollow;
};

#endif