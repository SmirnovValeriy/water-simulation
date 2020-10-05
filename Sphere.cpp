#include <glm/gtc/matrix_transform.hpp>
#include "Sphere.hpp"

Sphere::Sphere(float radius, uint sectors, uint stacks):
shader("shaders/sphereVertexShader.txt",
        "shaders/sphereGeometryShader.txt",
        "shaders/sphereFragmentShader.txt") {
    std::vector<GLfloat>vertices;
    std::vector<GLfloat>normals;
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float sectorStep = 2 * M_PI / sectors;
    float stackStep = M_PI / stacks;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stacks; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectors; ++j)
        {
            sectorAngle = j * sectorStep;
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(z);
            vertices.push_back(x);
            vertices.push_back(y);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(-nz);
            normals.push_back(nx);
            normals.push_back(-ny);
        }
    }

    std::vector<GLuint> indices;
    uint k1, k2;
    for(int i = 0; i < stacks; ++i)
    {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for(int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    count = indices.size();
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &positionVBO);
    glGenBuffers(1, &normalsVBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Sphere::draw(glm::vec3 pos, glm::mat4 viewMatrix,
        glm::mat4 projection, glm::vec4 lightColor, glm::vec3 lightPos, glm::vec4 sphereColor,
        glm::vec3 viewPos, float baseColorPart, float specularStrength) {
    shader.useProgram();
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos);
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, \
    glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, \
    glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, \
    glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shader.getProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform4f(glGetUniformLocation(shader.getProgram(), "sphereColor"), sphereColor.x, sphereColor.y, \
    sphereColor.z, sphereColor.a);
    glUniform4f(glGetUniformLocation(shader.getProgram(), "lightColor"), lightColor.x, lightColor.y, \
    lightColor.z, lightColor.a);
    glUniform1f(glGetUniformLocation(shader.getProgram(), "baseColorPart"), baseColorPart);
    glUniform3f(glGetUniformLocation(shader.getProgram(), "viewPos"), viewPos.x,
                viewPos.y, viewPos.z);
    glUniform1f(glGetUniformLocation(shader.getProgram(), "specularStrength"), specularStrength);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
