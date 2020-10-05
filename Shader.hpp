#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Info.hpp"


class Shader {
public:
    Shader(char const * vertexShaderPath,  char const * geometryShaderPath, char const * fragmentShaderPath);

    void useProgram();
    GLuint getProgram();
    void deleteProgram();

private:
    GLuint program;
};
