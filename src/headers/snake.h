#ifndef SNAKE_H
#define SNAKE_H

class Snake: public Actor{
public:
    Snake(Game* game);
    ~Snake();

    void updateActor(float delta) override;

    glm::vec2 getNewBodyPos();
    void addBody();
    void snakeDirChanged(){mDirChanged = true;}

    class CircleComponent* getCircle();

private:
    class CircleComponent* mCircle;
    std::vector<class Body*> mBodies;

    bool mDirChanged;
};

#endif