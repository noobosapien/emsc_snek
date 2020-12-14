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

    friend Matrix4 operator*(Matrix4& matA, Matrix4& matB){
        Matrix4 retVal;

        return retVal;
    }

    static const Matrix4 Identity;
};

#endif