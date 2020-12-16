#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <memory.h>
#include <limits>

class Vector2{
public:
    float x;
    float y;

    explicit Vector2(float x = 0, float y = 0){
        this->x = x;
        this->y = y;
    }

    void Set(float x, float y){
        this->x = x;
        this->y = y;
    }

    friend Vector2 operator+(const Vector2& a, const Vector2& b){
        return Vector2(a.x + b.x, a.y + b.y);
    }
    
    friend Vector2 operator-(const Vector2& a, const Vector2& b){
        return Vector2(a.x - b.x, a.y - b.y);
    }
    
    friend Vector2 operator*(const Vector2& a, const Vector2& b){
        return Vector2(a.x * b.x, a.y * b.y);
    }

    friend Vector2 operator*(const Vector2& a, const float scalar){
        return Vector2(a.x * scalar, a.y * scalar);
    }

    friend Vector2 operator*(const float scalar, const Vector2& a){
        return Vector2(a.x * scalar, a.y * scalar);
    }

    Vector2& operator*= (float scalar){
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }

    Vector2& operator+= (const Vector2& vec){
        this->x += vec.x;
        this->y += vec.y;
        return *this;
    }

    Vector2& operator-= (const Vector2& vec){
        this->x -= vec.x;
        this->y -= vec.y;
        return *this;
    }

    static const Vector2 Zero;
    static const Vector2 UnitX;
    static const Vector2 UnitY;
    static const Vector2 NegUnitX;
    static const Vector2 NegUnitY;

};

class Matrix4{
public:
    float mat[4][4];

    Matrix4(){
        *this = Matrix4::Identity;
    }

    explicit Matrix4(float inMat[4][4]){
        memcpy(mat, inMat, 16 * sizeof(float));
    }

    const float* getAsFloatPtr() const{
        return reinterpret_cast<const float*>(&mat[0][0]);
    }

    friend Matrix4 operator*(const Matrix4& matA, const Matrix4& matB){
        Matrix4 retVal;

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                retVal.mat[i][j] = 
                matA.mat[i][0] * matB.mat[0][j] +
                matA.mat[i][1] * matB.mat[1][j] +
                matA.mat[i][2] * matB.mat[2][j] +
                matA.mat[i][3] * matB.mat[3][j];
            }
        }

        return retVal;
    }

    Matrix4& operator*=(const Matrix4& mat){
        *this = *this * mat;
        return *this;
    }

    static Matrix4 createScale(float x, float y, float z){
        float temp[4][4] = {
            {x, 0.0f, 0.0f, 0.0f},
            {0.0f, y, 0.0f, 0.0f},
            {0.0f, 0.0f, z, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        return Matrix4(temp);
    }

    static Matrix4 createScale(float scale){
        return createScale(scale, scale, scale);
    }

    static Matrix4 createRotationZ(float theta){
        float temp[4][4] = {
            {cos(theta), sin(theta), 0.0f, 0.0f},
            {-sin(theta), cos(theta), 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };

        return Matrix4(temp);
    }

    static Matrix4 createTranslation(float x, float y, float z){
        float temp[4][4] = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {x, y, z, 1.0f}
        };

        return Matrix4(temp);
    }

    static Matrix4 createSimpleViewProj(float width, float height){
        float temp[4][4] = {
            {2/width, 0.f, 0.f, 0.f},
            {0.f, 2/height, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 1.f, 1.f}
        };

        return Matrix4(temp);
    }

    static const Matrix4 Identity;
};

#endif