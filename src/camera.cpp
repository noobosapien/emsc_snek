#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(float left, float right, float bot, float top): mPosition(0.f), mRotation(0.f),
mLeft(left), mRight(right), mTop(top), mBot(bot){
    mProjection = glm::ortho(mLeft, mRight, mBot, mTop, -1.f, 10.f);
    mView = glm::mat4(1.f);

    recomputeViewProj();
}

Camera::~Camera(){}

void Camera::recomputeViewProj(){
    glm::mat4 translate = glm::translate(glm::mat4(1.f), mPosition);
    translate = glm::rotate(translate, mRotation, glm::vec3(0.f, 0.f, 1.f));

    mView = glm::inverse(translate);

    mViewProj = mProjection * mView;
}

void Camera::addViewportVertices(float left, float right, float bot, float top){
    mLeft += left;
    mRight += right;
    mTop += top;
    mBot += bot;

    mProjection = glm::ortho(mLeft, mRight, mBot, mTop, -1.f, 10.f);

    recomputeViewProj();
}