#ifndef DEF_SHADER
#define DEF_SHADER

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include "ShaderUtils.h"
#include <glm/glm.hpp>

class Shader
{
    public:

    Shader(std::string vertexSource, std::string fragmentSource);
    ~Shader();

    GLuint getProgramID() const;

    void bindProjectionMatrix(glm::mat4 pMatrix);
    void bindViewMatrix(glm::mat4 vMatrix);

    private:

    void bindAttributes();

    GLuint m_vertexID;
    GLuint m_fragmentID;
    GLuint m_programID;

    GLint location_pMatrix;
    GLint location_vMatrix;

    std::string m_vertexSource;
    std::string m_fragmentSource;
};

#endif
