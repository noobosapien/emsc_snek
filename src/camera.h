#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"


class Camera{

public:
    Camera(float left = -.3f, float right = .3f, float bot = -.3f, float top = .3f);
    ~Camera();

    glm::mat4& getViewProj(){return mViewProj;}
    glm::mat4& getView(){return mView;}
    glm::mat4& getProjection(){return mProjection;}

    void setPosition(glm::vec3 pos){mPosition = pos; recomputeViewProj();}
    void setRotation(float rot){mRotation = rot; recomputeViewProj();}

    void addViewportVertices(float left, float right, float top, float bot);

private:
    void recomputeViewProj();

    glm::mat4 mViewProj;
    glm::mat4 mView;
    glm::mat4 mProjection;

    glm::vec3 mPosition;
    float mRotation;

    float mLeft;
    float mRight;
    float mTop;
    float mBot;
};

#endif