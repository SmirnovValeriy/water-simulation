cmake_minimum_required(VERSION 3.6)
project(WaterSim)

set(CMAKE_CXX_STANDARD 11)

set(MY_TARGET WaterSim)

set(SOURCE_FILES main.cpp Solver.cpp Solver.hpp Renderer.cpp Renderer.hpp Info.hpp Info.cpp View.cpp View.hpp
        Shader.hpp Shader.cpp Environment.hpp Environment.cpp Sphere.cpp Sphere.hpp)
add_executable(${MY_TARGET} ${SOURCE_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pool2_600_300.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pool2_600_600.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(X11 REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")


target_include_directories(${MY_TARGET} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${MY_TARGET} glfw ${OPENGL_gl_LIBRARY} GLEW X11 pthread SOIL)

