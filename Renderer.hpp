#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "SOIL.h"
#include "Shader.hpp"
#include "Info.hpp"
#include "View.hpp"
#include "Sphere.hpp"


class Renderer {
public:
    Renderer(size_t xCells, size_t yCells, uint screenWidth, uint screenHeight, GLFWwindow * window, GLfloat perspectiveRatio);
    ~Renderer();
    void render(GLuint waterID);

protected:
    View view;
    GLfloat perspectiveRatio;
    GLFWwindow * window;
    uint screenWidth;
    uint screenHeight;
    glm::vec3 lightPos;
    GLuint xCells;
    GLuint yCells;
    double step;
    Sphere sphere;


    Shader waterShader;
    Shader borderShader;
    Shader lightShader;

    GLuint textureBottom;
    GLuint textureBorder;
    GLuint waterVAO;
    GLuint waterVBO;
    GLuint waterEBO;

    GLuint bottomVAO;
    GLuint bottomVBO;
    GLuint bottomEBO;

    GLuint borderVAO;
    GLuint borderVBO;
    GLuint borderEBO;

    GLuint lightVAO;
    GLuint lightVBO;
    GLuint lightEBO;
};
