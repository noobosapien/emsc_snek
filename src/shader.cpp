#include "shader.h"

Shader::Shader(): mVertexShader(0), mFragShader(0), mShaderProgram(0), VAO(0), VBO(0)
{
}

Shader::~Shader()
{
}

bool Shader::load(const std::string& vertName, const std::string& fragName){
    if(!compileShader(vertName, GL_VERTEX_SHADER, mVertexShader) || !compileShader(fragName, GL_FRAGMENT_SHADER, mFragShader)){
        return false;
    }

    mShaderProgram = glCreateProgram();

    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragShader);
    glLinkProgram(mShaderProgram);

    if(!isValidProgram())
        return false;
    
    return true;
}

void Shader::unLoad(){
    glDeleteProgram(mShaderProgram);
    glDeleteShader(mVertexShader);
    glDeleteShader(mFragShader);
}

unsigned int Shader::setActive(){
    glUseProgram(mShaderProgram);
    glBindVertexArray(VAO);
    return mShaderProgram;
}

void Shader::setBufferSubData(float* verts, unsigned int numVerts, unsigned int cols){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numVerts * cols, verts);
}

void Shader::setAttrib(const char* name, unsigned int size, unsigned int stride,
    unsigned int offset){

    setActive();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLint a_location;
    a_location = glGetAttribLocation(mShaderProgram, name);
    glEnableVertexAttribArray(a_location);

    glVertexAttribPointer(a_location, size, GL_FLOAT, GL_FALSE, sizeof(float) * stride, reinterpret_cast<void*>(sizeof(float) * offset));
}


void Shader::setMatrixUniform(const char* name, const glm::mat4& matrix){
    setActive();
    GLuint loc = glGetUniformLocation(mShaderProgram, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3Uniform(const char* name, const glm::vec3& vec){
    setActive();
    GLuint loc = glGetUniformLocation(mShaderProgram, name);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::setFloatUniform(const char* name, const float fl){
    setActive();
    GLuint loc = glGetUniformLocation(mShaderProgram, name);
    glUniform1f(loc, fl);
}

void Shader::setVec2Uniform(const char* name, const glm::vec2& vec){
    setActive();
    GLuint loc = glGetUniformLocation(mShaderProgram, name);
    glUniform2f(loc, vec.x, vec.y);
}

bool Shader::compileShader(std::string filename, GLenum shaderType, GLuint& outShader){
    std::ifstream shaderFile(filename);

    if(shaderFile.is_open()){
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();

        std::string contents = sstream.str();

        const char* contentsChar = contents.c_str();

        outShader = glCreateShader(shaderType);

        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if(!isCompiled(outShader)){
            printf("Failed to compile shader %s\n", filename.c_str());
            return false;
        }
    }else{
        printf("Shader file not found %s\n", filename.c_str());
        return false;
    }

    return true;
}

bool Shader::isCompiled(GLuint shader){
    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE){
        char buffer[512];
        memset(buffer, 0, 512);

        glGetShaderInfoLog(shader, 511, nullptr, buffer);

        printf("GLSL compile failed: %s\n", buffer);
        return false;
    }

    return true;
}

bool Shader::isValidProgram(){
    GLint status;

    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);

    if(status != GL_TRUE){
        char buffer[512];
        memset(buffer, 0, 512);

        glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);

        printf("GLSL link failed: %s\n", buffer);
        return false;
    }

    return true;
}

void Shader::setVertexData(float* verts, unsigned int numVerts, 
    const unsigned int* indices, unsigned int numIndices, unsigned int cols, GLenum usage){

    setActive();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVerts * cols, verts, usage);
    
    
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, usage);
    
}
