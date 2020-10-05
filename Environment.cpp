#include "Environment.hpp"


GLEnvironment::GLEnvironment(int width, int height):
        window(nullptr),
        perspectiveRatio((GLfloat) width / (GLfloat) height) {

    if(!glfwInitialized) {
        glfwInitialized = glfwInit();
        if (!glfwInitialized) {
            std::cout << "Error occured initializing GLFW\n" << std::endl;
            return;
        }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(width, height, "Water Simulation", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Error occurred creating window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    //necessary for OpenGL core-profile using
    glewExperimental = GL_TRUE;
    //init GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        std::cout << "Error occurred initializing GLEW" << std::endl;
        return;
    }

    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);
    glfwSetKeyCallback(window, View::keyCallback);
    glfwSetCursorPosCallback(window, View::mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

}

GLboolean GLEnvironment::glfwInitialized = false;

GLEnvironment::~GLEnvironment() {
    if (glfwInitialized) {
        glfwTerminate();
        glfwInitialized = false;
    }
}
