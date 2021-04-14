#ifndef BORDERCOMPONENT_H
#define BORDERCOMPONENT_H

class BorderComponent: public Component{

public:
    BorderComponent(Actor* owner, int drawOrder = 150);
    ~BorderComponent();

    void draw(Shader* shader);

    int getDrawOrder(){return mDrawOrder;}

private:
    int mDrawOrder;
};

#endif