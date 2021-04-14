#ifndef FOOD_H
#define FOOD_H

class Food: public Actor{

public:

    enum FoodAction{
        FA_END,
        FA_SETPOS,
        FA_SETSCALE
    };

    Food(Game* game);
    ~Food();

    void read(InputStream& inStream);

    void readAndSetPos(InputStream& inStream);
    void readAndSetScale(InputStream& inStream);

    virtual void updateActor(float delta) override;

private:
    CircleComponent* mCircle;
    Snake* mSnake;
};

#endif