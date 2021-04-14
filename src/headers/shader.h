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
    void setVec3Uniform(const char* name, const glm::vec3& vec);

    void setVertexData(float* verts, unsigned int numVerts, 
    const unsigned int* indices, unsigned int numIndices, unsigned int cols, GLenum usage = GL_STATIC_DRAW);
    void setAttrib(const char* name, unsigned int size, unsigned int stride,
    unsigned int offset);
    void setBufferSubData(float* verts, unsigned int numVerts, unsigned int cols);

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