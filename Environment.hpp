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
#include "View.hpp"


class GLEnvironment {
public:
    GLEnvironment(int width, int height);
    ~GLEnvironment();

    GLFWwindow * window;
    static GLboolean glfwInitialized;
    GLfloat perspectiveRatio;
};
