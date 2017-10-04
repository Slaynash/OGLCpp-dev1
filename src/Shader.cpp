#include "Shader.h"

Shader::Shader(std::string vertexSource, std::string fragmentSource) : m_vertexID(0), m_fragmentID(0), m_programID(0), m_vertexSource(vertexSource), m_fragmentSource(fragmentSource){
    if(loadShader(m_vertexSource, m_fragmentSource, m_programID, m_vertexID, m_fragmentID)){
        bindAttributes();
        linkShader(m_programID);
        glUseProgram(m_programID);

        location_pMatrix = glGetUniformLocation(m_programID, "pMatrix");
        location_vMatrix = glGetUniformLocation(m_programID, "vMatrix");
    }
}

Shader::~Shader(){
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}

void Shader::bindProjectionMatrix(glm::mat4 pMatrix){
    glUniformMatrix4fv(location_pMatrix, 1, GL_FALSE, &pMatrix[0][0]);
}

void Shader::bindViewMatrix(glm::mat4 vMatrix){
    glUniformMatrix4fv(location_vMatrix, 1, GL_FALSE, &vMatrix[0][0]);
}

void Shader::bindAttributes(){
    glBindAttribLocation(m_programID, 0, "in_Vertex");
    glBindAttribLocation(m_programID, 1, "in_TexCoord");
}

GLuint Shader::getProgramID() const {
    return m_programID;
}