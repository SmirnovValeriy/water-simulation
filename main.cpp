#include <iostream>
#define GLEW_STATIC
//GLEW manages functions pointers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Solver.hpp"
#include "Renderer.hpp"
#include "Environment.hpp"

#define xCells 255
#define yCells 255
#define screenHeight 600
#define screenWidth 800

int main() {

    //create GL environment
    GLEnvironment env(screenWidth, screenHeight);

    //create Renderer with constructor
    Renderer renderer(xCells, yCells, screenWidth, screenHeight, env.window, env.perspectiveRatio);

    //create Solver with constructor
    Solver solver(xCells, yCells, 0.4);

    size_t counter = 0;
    bool drop = true;
    bool pause = false;
    while(!glfwWindowShouldClose(env.window)) {
        if(!pause) {
            solver.run(drop);
        }
        renderer.render(solver.getWaterID());
        View::applyCallbacks(++counter, drop, pause);
    }
}
