
#include "View.hpp"

const GLfloat YAW        =  45.0f;
const GLfloat PITCH      =   -40.0f;
const GLfloat SPEED      =   1.0f;
const GLfloat SENSITIVTY =   0.15f;

View::View()= default;

void View::keyCallback(GLFWwindow *win, int key, int scan, int action, int mode) {
    switch(key) {
        case GLFW_KEY_ESCAPE:
            if(action == GLFW_PRESS)
                glfwSetWindowShouldClose(win, GL_TRUE);
            break;
        case GLFW_KEY_F5:
            if(action == GLFW_RELEASE)
                pause = !pause;
            break;
        case GLFW_KEY_Q:
            if(action == GLFW_RELEASE)
                wireframe = !wireframe;
            break;
        case GLFW_KEY_SPACE:
            if(action == GLFW_PRESS)
                drop = true;
            break;
        case GLFW_KEY_W:
            if(action == GLFW_PRESS) keys[GLFW_KEY_W] = true;
            else if (action == GLFW_RELEASE) keys[GLFW_KEY_W] = false;
            break;
        case GLFW_KEY_A:
            if(action == GLFW_PRESS) keys[GLFW_KEY_A] = true;
            else if (action == GLFW_RELEASE) keys[GLFW_KEY_A] = false;
            break;
        case GLFW_KEY_S:
            if(action == GLFW_PRESS) keys[GLFW_KEY_S] = true;
            else if (action == GLFW_RELEASE) keys[GLFW_KEY_S] = false;
            break;
        case GLFW_KEY_D:
            if(action == GLFW_PRESS) keys[GLFW_KEY_D] = true;
            else if (action == GLFW_RELEASE) keys[GLFW_KEY_D] = false;
            break;
    }
}

void View::updateCameraVectors() {
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraRight = glm::normalize(glm::cross(cameraFront, Up));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void View::mouseCallback(GLFWwindow *win, double xpos, double ypos) {
    static GLfloat lastX = 400.0f, lastY = 300.0f;
    static bool firstMouseMove = true;
    if (firstMouseMove) {
        lastX = (GLfloat)xpos;
        lastY = (GLfloat)ypos;
        firstMouseMove = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensetivity;
    yoffset *= mouseSensetivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void View::applyCallbacks(size_t counter, bool &d, bool &p) {
    GLfloat offset = movementSpeed * dt;
    if(keys[GLFW_KEY_W]) {
        cameraPos += cameraFront * offset;
    }
    if(keys[GLFW_KEY_A]) {
        cameraPos -= cameraRight * offset;
    }
    if(keys[GLFW_KEY_S]) {
        cameraPos -= cameraFront * offset;
    }
    if(keys[GLFW_KEY_D]) {
        cameraPos += cameraRight * offset;
    }
    if(drop || counter % 200 == 0) {
        d = true;
        drop = false;
    }
    else d = false;
    if(wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    p = pause;
}

glm::mat4 View::getViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::vec3 View::getCameraPos() {
    return cameraPos;
}

void View::set_dt(GLfloat d) {
    dt = d - last;
}

void View::set_last(GLfloat d) {
    last = d;
}

GLfloat View::yaw(YAW);
GLfloat View::pitch(PITCH);
GLfloat View::movementSpeed(SPEED);
GLfloat View::mouseSensetivity(SENSITIVTY);
glm::vec3 View::Up = glm::vec3(0.0f, 1.f, 0.0f);
glm::vec3 View::cameraPos = glm::vec3(-0.5f, 1.5f,  -0.5f);
glm::vec3 View::cameraEye = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 View::cameraFront(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
glm::vec3 View::cameraRight(glm::normalize(glm::cross(cameraFront, Up)));
glm::vec3 View::cameraUp(glm::normalize(glm::cross(cameraRight, cameraFront)));
GLfloat View::dt = 0.f;
GLfloat View::last = glfwGetTime();
bool View::keys[1024];
bool View::wireframe(false);
bool View::drop(false);
bool View::pause(false);
