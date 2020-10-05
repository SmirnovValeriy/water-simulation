#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL.h"


/* shader info */
void printShaderLog(const GLuint & shader);

/*program info */
void printProgramLog(const GLuint & program);

/*framebuffer info */
void printFramebufferLog();

/*load texture info */
void printTextureLoadLog();

/* initializing 2D texture info */
void printTexture2DInitLog();
