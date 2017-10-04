#ifndef DEF_SHADERUTILS
#define DEF_SHADERUTILS

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>

bool loadShader(std::string vertexSource, std::string fragmentSource, GLuint &programID, GLuint &vertexID, GLuint &fragmentID);
bool linkShader(GLuint programID);
bool compileShader(GLuint &shader, GLenum type, std::string const &sourceFile);

#endif