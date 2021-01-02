#ifndef BORDERCOMPONENT_H
#define BORDERCOMPONENT_H

#include "component.h"
#include "actor.h"
#include "shader.h"
#include "game.h"
#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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