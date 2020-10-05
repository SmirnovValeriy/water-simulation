#include "Solver.hpp"

#define w 1.985
#define MIN_DROP_RADIUS 5
#define MAX_DROP_RADIUS 5

void rotate(Framebuffer &a, Framebuffer &b, Framebuffer &c) {
    Framebuffer tmp = a;
    a = b;
    b = c;
    c = tmp;
}

Framebuffer::Framebuffer(size_t xCells, size_t yCells, float baseLevel): water(xCells * yCells * 4, baseLevel) {

    /* initialize texture */
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, xCells, yCells, 0, GL_RGBA, GL_FLOAT, &water[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* initialize framebuffer objects */
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
    printFramebufferLog();
}

Framebuffer& Framebuffer::operator=(Framebuffer * const copyFrom){
    texID = copyFrom->texID;
    FBO = copyFrom->FBO;
    water = copyFrom->water;
    return *this;
}

GLuint Framebuffer::getTexID() {
    return texID;
}

void Framebuffer::bindTexture() {
    glActiveTexture(GL_TEXTURE0 + texID);
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Framebuffer::unbindTexture() {
    glActiveTexture(GL_TEXTURE0 + texID);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::bindBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
}

void Framebuffer::unbindBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Solver::Solver(size_t xCells, size_t yCells, float baseLevel):
previousWater(xCells, yCells, baseLevel),
currentWater(xCells, yCells, baseLevel),
nextWater(xCells, yCells, baseLevel),
shader("shaders/FramebufferVertexShader.txt",
        "shaders/FramebufferGeometryShader.txt",
        "shaders/FramebufferFragmentShader.txt"),
xCells(xCells), yCells(yCells),
step(std::min(1. / (xCells), 1. / (yCells))) {
    std::vector<GLfloat> mesh(2 * xCells * yCells);
    std::vector<GLuint> meshIndices((xCells) * (yCells));

    for (size_t x = 0, index = 0; x < xCells; ++x) {
        for (size_t y = 0; y < yCells; ++y) {
            mesh[index++] = x * step;
            mesh[index++] = y * step;
        }
    }
    for (size_t x = 0, index = 0; x < xCells; ++x) {
        for (size_t y = 0; y < yCells; ++y) {
            meshIndices[index] = index++;
        }
    }
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(GLfloat), mesh.data(), GL_STREAM_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(GLuint), meshIndices.data(), GL_STREAM_DRAW);
    glBindVertexArray(0);
}

void Solver::run(GLboolean drop) {
    glClear(GL_COLOR_BUFFER_BIT);
    GLfloat center[2];
    GLfloat radius = 0.0;
    GLfloat W = w;
    GLuint program = shader.getProgram();

    if(drop){
        center[0] = (rand() % xCells) * step;
        center[1] = (rand() % yCells) * step;
        radius = (rand() % MIN_DROP_RADIUS + MAX_DROP_RADIUS) * step;
    }

    nextWater.bindBuffer();
    glViewport(0, 0, xCells, yCells);
    shader.useProgram();
    glUniform1f(glGetUniformLocation(program, "w"), W);
    glUniform2f(glGetUniformLocation(program, "center"), center[0], center[1]);
    glUniform1f(glGetUniformLocation(program, "radius"), radius);
    glUniform1i(glGetUniformLocation(program, "drop"), drop);
    glUniform1f(glGetUniformLocation(program, "step"), step);
    previousWater.bindTexture();
    glUniform1i(glGetUniformLocation(program, "previousWater"), previousWater.getTexID());
    currentWater.bindTexture();
    glUniform1i(glGetUniformLocation(program, "currentWater"), currentWater.getTexID());
    GLint polygonMode;
    glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(VAO);
    glDrawElements(GL_POINTS, (xCells) * (yCells), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    previousWater.unbindTexture();
    currentWater.unbindTexture();
    nextWater.unbindBuffer();

    rotate(previousWater, currentWater, nextWater);
}

GLuint Solver::getWaterID() {
    return currentWater.getTexID();
}