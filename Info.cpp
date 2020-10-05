#include "Info.hpp"


void printShaderLog(const GLuint & shader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Error occurred compiling shader:\n" << infoLog << std::endl;
    }
    else std::cout << "Success" << std::endl;
}

void printProgramLog(const GLuint & program) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Error occurred building program:\n" << infoLog << std::endl;
    }
}

void printFramebufferLog() {
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void printTextureLoadLog() {
    std::cout << std::endl << "Texture Image Load Status:" << std::endl;
    std::cout << SOIL_last_result() << std::endl << std::endl;
}

void printTexture2DInitLog() {
    std::cout << std::endl << "Texture 2D Initializing Status:" << std::endl;
    std::cout << glGetError() << std::endl << std::endl;
}
