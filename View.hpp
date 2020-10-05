#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


class View {
public:
    View();
    static void keyCallback(GLFWwindow * win, int key, int scan, int action, int mode);
    static void updateCameraVectors();
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    glm::vec3 getCameraPos();
    glm::mat4 getViewMatrix();
    void set_last(GLfloat d);
    void set_dt(GLfloat d);
    static bool keys[1024];
    static void applyCallbacks(size_t counter, bool & drop, bool &p);
private:
/* we use these member variables in static member functions so we need them to be to static */
    static glm::vec3 cameraPos;
    static glm::vec3 cameraEye;
    static glm::vec3 Up;
    static glm::vec3 cameraUp;
    static glm::vec3 cameraFront;
    static glm::vec3 cameraRight;
    static GLfloat yaw;
    static GLfloat pitch;
    static GLfloat movementSpeed;
    static GLfloat mouseSensetivity;
    static GLfloat last;
    static GLfloat dt;
    static bool wireframe;
    static bool drop;
    static bool pause;
};
