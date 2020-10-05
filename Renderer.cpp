#include "Renderer.hpp"
#define BORDER_HEIGHT 0.5


Renderer::Renderer(size_t xCells, size_t yCells, uint screenWidth, uint screenHeight, \
GLFWwindow * window, GLfloat perspectiveRatio):
window(window),
screenWidth(screenWidth),
screenHeight(screenHeight),
perspectiveRatio(perspectiveRatio),
xCells(xCells), yCells(yCells),
step(std::min(1. / (xCells - 1), 1. / (yCells - 1))),
lightPos(0.5, 1., 0.5),
sphere(0.1, 100, 200),
waterShader("shaders/surfaceVertexShader.txt",
              "shaders/surfaceGeometryShader.txt",
        "shaders/surfaceFragmentShader.txt"),
borderShader("shaders/borderVertexShader.txt",
             "shaders/borderGeometryShader.txt",
             "shaders/borderFragmentShader.txt"),
lightShader("shaders/lightVertexShader.txt",
            "shaders/lightGeometryShader.txt",
            "shaders/lightFragmentShader.txt") {

    /* create water mesh */
    std::vector<GLfloat> mesh(2 * xCells * yCells);
    for (size_t x = 0, index = 0; x < xCells; ++x) {
        for (size_t y = 0; y < yCells; ++y) {
            mesh[index++] = x * step;
            mesh[index++] = y * step;
        }
    }
    std::vector<GLuint> meshIndices(2 * (xCells - 1) * yCells);
    for (size_t x = 0, index = 0; x < xCells - 1; ++x) {
        for (size_t y = 0; y < yCells; ++y) {
            meshIndices[index++] = x * yCells + y;
            meshIndices[index++] = (x + 1) * yCells + y;
        }
    }

    glGenVertexArrays(1, &waterVAO);
    glBindVertexArray(waterVAO);
    glGenBuffers(1, &waterVBO);
    glGenBuffers(1, &waterEBO);
    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(GLfloat), mesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(GLuint), meshIndices.data(), GL_STREAM_DRAW);
    glBindVertexArray(0);
    /*-------------------*/

    /* create border primitives */
    GLfloat begin = mesh[0];
    GLfloat end = mesh[2 * xCells * yCells - 1];
    std::vector<GLfloat> meshBorder = {
            begin, 0., begin, 0., 0.,
            begin, 0., end, 1., 0.,
            begin, BORDER_HEIGHT, begin, 0., 1.,
            begin, BORDER_HEIGHT, end, 1., 1.,

            begin, 0., end, 0., 0.,
            end, 0., end, 1., 0.,
            begin, BORDER_HEIGHT, end, 0., 1.,
            end, BORDER_HEIGHT, end, 1., 1.,

            end, 0., end, 0., 0.,
            end, 0., begin, 1., 0.,
            end, BORDER_HEIGHT, end, 0., 1.,
            end, BORDER_HEIGHT, begin, 1., 1.,

            end, 0., begin, 0., 0.,
            begin, 0., begin, 1., 0.,
            end, BORDER_HEIGHT, begin, 0., 1.,
            begin, BORDER_HEIGHT, begin,  1., 1.,
    };
    std::vector<GLuint> borderIndices = {
            0, 1, 2,
            3, 2, 1,
            4, 5, 6,
            7, 6, 5,
            8, 9, 10,
            11, 10, 9,
            12, 13, 14,
            15, 14, 13
    };


    glGenVertexArrays(1, &borderVAO);
    glBindVertexArray(borderVAO);
    glGenBuffers(1, &borderVBO);
    glGenBuffers(1, &borderEBO);
    glGenTextures(1, &textureBorder);
    glBindTexture(GL_TEXTURE_2D, textureBorder);

    int texBorderWidth, texBorderHeight;
    unsigned char* image = SOIL_load_image("pool2_600_300.png", \
    &texBorderWidth, &texBorderHeight, 0, SOIL_LOAD_RGB);
    printTextureLoadLog();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texBorderWidth, texBorderHeight, 0, \
    GL_RGB, GL_UNSIGNED_BYTE, image);
    printTexture2DInitLog();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ARRAY_BUFFER, borderVBO);
    glBufferData(GL_ARRAY_BUFFER, meshBorder.size() * sizeof(GLfloat), meshBorder.data(), GL_STATIC_DRAW);
    //coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, borderIndices.size() * sizeof(GLuint), borderIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    /*-------------------*/

    /* create bottom primitives */
    std::vector<GLfloat> meshBottom = {
            begin, 0., begin, 0., 0.,
            begin, 0., end, 1., 0.,
            end, 0, begin, 0., 1.,
            end, 0, end, 1., 1.,
    };
    std::vector<GLuint> bottomIndices = {
            2, 1, 0,
            1, 2, 3,
    };

    glGenVertexArrays(1, &bottomVAO);
    glBindVertexArray(bottomVAO);
    glGenBuffers(1, &bottomVBO);
    glGenBuffers(1, &bottomEBO);
    glGenTextures(1, &textureBottom);
    glBindTexture(GL_TEXTURE_2D, textureBottom);

    int texBottomWidth, texBottomHeight;
    unsigned char* imageBottom = SOIL_load_image("pool2_600_600.png", \
    &texBottomWidth, &texBottomHeight, 0, SOIL_LOAD_RGB);
    printTextureLoadLog();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texBottomWidth, texBottomHeight, 0, \
    GL_RGB, GL_UNSIGNED_BYTE, imageBottom);
    printTexture2DInitLog();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(imageBottom);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ARRAY_BUFFER, bottomVBO);
    glBufferData(GL_ARRAY_BUFFER, meshBottom.size() * sizeof(GLfloat), meshBottom.data(), GL_STATIC_DRAW);
    //coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bottomIndices.size() * sizeof(GLuint), bottomIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    /*-------------------*/

    /* create light primitive */
    std::vector<GLfloat> meshLight = {
            0.0, 0.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 0.0, 1.0,
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 1.0, 0.0,
    };

    std::vector<GLuint> lightIndices = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            0, 1, 5,
            0, 5, 4,
            1, 2, 6,
            1, 6, 5,
            2, 3, 7,
            2, 7, 6,
            3, 0, 4,
            3, 4, 7
    };
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, meshLight.size() * sizeof(GLfloat), meshLight.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightIndices.size() * sizeof(GLuint), lightIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    /*-------------------*/
}

void Renderer::render(GLuint waterID) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    /* set up window */
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    glfwPollEvents();
    /*-------------------*/

    /* what time is it? */
    GLfloat current = glfwGetTime();
    view.set_dt(current);
    view.set_last(current);
    /*-------------------*/

    /* some parameters */
    glm::vec4 lightColor(1.0f);
    float baseColorPart(0.1f);
    float specularStrength(0.3f);
    glm::vec4 waterColor(0., 0.5, 0.5, 0.5);
    glm::vec3 spherePos(0.8, 0.1, 0.8);
    glm::vec4 sphereColor(0.0, 0.5, 0.8, 1.0);
    glm::vec3 viewPos = view.getCameraPos();
    /*-------------------*/

    /* set up view parameters */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix(1.0);
    viewMatrix = view.getViewMatrix();
    glm::mat4 projection(1.0);
    projection = glm::perspective(glm::radians(45.0f), perspectiveRatio, 0.1f, 100.0f);
    /*-------------------*/

    /* draw light primitive */
    lightShader.useProgram();
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, lightPos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
    glUniformMatrix4fv(glGetUniformLocation(lightShader.getProgram(), "model"), 1, GL_FALSE, \
    glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(lightShader.getProgram(), "view"), 1, GL_FALSE, \
    glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(lightShader.getProgram(), "projection"), 1, GL_FALSE, \
    glm::value_ptr(projection));
    glBindVertexArray(lightVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    /*-------------------*/

    /* draw border primitive */
    borderShader.useProgram();

    glUniform3f(glGetUniformLocation(borderShader.getProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(glGetUniformLocation(borderShader.getProgram(), "baseColorPart"), baseColorPart);
    glUniform4f(glGetUniformLocation(borderShader.getProgram(), "lightColor"), lightColor.x, lightColor.y, \
    lightColor.z, lightColor.a);
    glUniformMatrix4fv(glGetUniformLocation(borderShader.getProgram(), "view"), 1, GL_FALSE, \
    glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(borderShader.getProgram(), "projection"), 1, GL_FALSE, \
    glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBorder);
    glBindVertexArray(borderVAO);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    /*-------------------*/

    /* draw border primitive */
    borderShader.useProgram();

    glUniform3f(glGetUniformLocation(borderShader.getProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(glGetUniformLocation(borderShader.getProgram(), "baseColorPart"), baseColorPart);
    glUniform4f(glGetUniformLocation(borderShader.getProgram(), "lightColor"), lightColor.x, lightColor.y, \
    lightColor.z, lightColor.a);
    glUniformMatrix4fv(glGetUniformLocation(borderShader.getProgram(), "view"), 1, GL_FALSE, \
    glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(borderShader.getProgram(), "projection"), 1, GL_FALSE, \
    glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBottom);
    glBindVertexArray(bottomVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    /*-------------------*/

    /* render sphere */
    sphere.draw(spherePos, viewMatrix, projection, lightColor, lightPos, sphereColor,
            viewPos, baseColorPart, specularStrength);

    /* render water */
    waterShader.useProgram();

    //get water height map from solver
    glActiveTexture(GL_TEXTURE0 + waterID);
    glBindTexture(GL_TEXTURE_2D, waterID);
    glUniform1i(glGetUniformLocation(waterShader.getProgram(), "currentWater"), waterID);

    glUniform3f(glGetUniformLocation(waterShader.getProgram(), "viewPos"), viewPos.x,
            viewPos.y, viewPos.z);
    glUniform1f(glGetUniformLocation(waterShader.getProgram(), "specularStrength"), 1.f);
    glUniform3f(glGetUniformLocation(waterShader.getProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(glGetUniformLocation(waterShader.getProgram(), "baseColorPart"), baseColorPart);
    glUniform4f(glGetUniformLocation(waterShader.getProgram(), "objectColor"), waterColor.x, waterColor.y, \
    waterColor.z, waterColor.a);
    glUniform4f(glGetUniformLocation(waterShader.getProgram(), "lightColor"), lightColor.x, lightColor.y, \
    lightColor.z, lightColor.a);
    glUniformMatrix4fv(glGetUniformLocation(waterShader.getProgram(), "view"), 1, GL_FALSE, \
    glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(waterShader.getProgram(), "projection"), 1, GL_FALSE, \
    glm::value_ptr(projection));
    glBindVertexArray(waterVAO);
    for (GLuint row = 0; row < xCells - 1; ++row) {
        glDrawElements(GL_TRIANGLE_STRIP, yCells * 2, GL_UNSIGNED_INT, \
        (GLvoid*)(row * yCells * 2 * sizeof(GLuint)));
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    /*-------------------*/

    glfwSwapBuffers(window);
}

Renderer::~Renderer() {
    waterShader.deleteProgram();
    lightShader.deleteProgram();
    borderShader.deleteProgram();
    glDeleteVertexArrays(1, &waterVAO);
    glDeleteVertexArrays(1, &bottomVAO);
    glDeleteBuffers(1, &waterVBO);
    glDeleteBuffers(1, &bottomVBO);
}
