#include "Shader.hpp"


Shader::Shader(char const * vertexShaderPath, char const * geometryShaderPath, char const * fragmentShaderPath) {
    std::ifstream vertexShaderFile;
    std::ifstream geometryShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vertexShaderSource, geometryShaderSource, fragmentShaderSource;

    try {
        vertexShaderFile.open(vertexShaderPath);
        geometryShaderFile.open(geometryShaderPath);
        fragmentShaderFile.open(fragmentShaderPath);

        std::stringstream vertexShaderStream, geometryShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        geometryShaderStream << geometryShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderSource = vertexShaderStream.str();
        geometryShaderSource = geometryShaderStream.str();
        fragmentShaderSource = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error occured reading shaders files";
    }

    GLuint vertexShader, geometryShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char * vertexShaderSourceChar = vertexShaderSource.c_str();
    const char * geometryShaderSourceChar = geometryShaderSource.c_str();
    const char * fragmentShaderSourceChar = fragmentShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceChar, NULL);
    glShaderSource(geometryShader, 1, &geometryShaderSourceChar, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceChar, NULL);

    glCompileShader(vertexShader);
    glCompileShader(geometryShader);
    glCompileShader(fragmentShader);

    printShaderLog(vertexShader);
    printShaderLog(geometryShader);
    printShaderLog(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, geometryShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    printProgramLog(program);

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}

void Shader::useProgram() {
    glUseProgram(program);
}

GLuint Shader::getProgram(){
    return program;
}
void Shader::deleteProgram() {
    glDeleteProgram(program);
}
