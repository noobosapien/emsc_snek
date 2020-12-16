#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <GLES2/gl2.h>
#include <GL/glfw.h>

#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>

#include "math.h"

#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
    Shader();
    ~Shader();

    bool load(const std::string& vertName, const std::string& fragName);
    void unLoad();

    void setActive();
    void setMatrixUniform(const char* name, const Matrix4& matrix);
    void setVertexData(float* verts, unsigned int numVerts, 
    const unsigned int* indices, unsigned int numIndices);
    void setAttrib(const char* name, unsigned int size, unsigned int stride,
    unsigned int offset);

private:
    bool compileShader(std::string filename, GLenum shaderType, GLuint& outShader);
    bool isCompiled(GLuint shader);
    bool isValidProgram();
    
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;

    GLuint VBO;
    GLuint IBO;
};

#endif