
#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"

class Sphere {
    GLuint VAO;
    GLuint positionVBO;
    GLuint normalsVBO;
    GLuint EBO;
    uint count;
    Shader shader;

public:
    Sphere(float radius, uint sectors, uint stacks);

    void draw(glm::vec3 pos, glm::mat4 viewMatrix,
                      glm::mat4 projection, glm::vec4 lightColor, glm::vec3 lightPos, glm::vec4 sphereColor,
                      glm::vec3 viewPos, float baseColorPart, float specularStrength);
};