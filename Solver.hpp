#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <cstddef>
#include <cmath>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include "SOIL.h"
#include "Info.hpp"

void rotate(GLuint a, GLuint b, GLuint c);

class Framebuffer {
    GLuint texID;
    GLuint FBO;
    std::vector<GLfloat> water;
public:
    Framebuffer(size_t xCells, size_t yCells, float baseLevel);
    Framebuffer& operator=(Framebuffer * const copyFrom);
    GLuint getTexID();
    void bindTexture();
    void bindBuffer();
    void unbindTexture();
    void unbindBuffer();
};

class Solver {
    Framebuffer previousWater;
    Framebuffer currentWater;
    Framebuffer nextWater;
    Shader shader;
    size_t xCells;
    size_t yCells;
    float step;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
public:
    Solver(size_t xCells, size_t yCells, float baseLevel = 0.2);
    void run(GLboolean drop);
    GLuint getWaterID();
};
