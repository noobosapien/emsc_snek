#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <GLES2/gl2.h>
#include <GL/glfw.h>
#include <GLES3/gl3.h>

#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
    Shader();
    ~Shader();

    bool load(const std::string& vertName, const std::string& fragName);
    void unLoad();

    unsigned int setActive();

    void setMatrixUniform(const char* name, const glm::mat4& matrix);
    void setFloatUniform(const char* name, const float fl);
    void setVec2Uniform(const char* name, const glm::vec2& vec);

    void setVertexData(float* verts, unsigned int numVerts, 
    const unsigned int* indices, unsigned int numIndices, unsigned int cols);
    void setAttrib(const char* name, unsigned int size, unsigned int stride,
    unsigned int offset);

private:
    bool compileShader(std::string filename, GLenum shaderType, GLuint& outShader);
    bool isCompiled(GLuint shader);
    bool isValidProgram();
    
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;

    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
};

#endif