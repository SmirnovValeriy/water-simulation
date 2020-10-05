## Water Simulation

### Water simulation task requirements
During a Computer Graphics course students of the Computional Mathematics and Cybernetics Faculty of Moscow State University are offered to solve a task on water simulation algorithm create a scene using the algorithm.

Obligatory requiremnts are as following:
1. using at least 3 different objects (pool walls and bottom are 2 different objects);
2. using at least 1 light source;
3. realistic lighting;
4. water should be transparent;
5. controlling with keys.

Students are also offered to perform additional features.

I implemented more complex version of the task using OpenGL3, **shaders** (besides basic vertex and fragment shaders I also use geometric shader) and calculations on a **GPU**.

### Scene
![water-simulation.gif](water-simulation.gif)

**Controlling:**
1. WASD - camera moving;
2. mouse - camera turns (as in shooter games);
3. SPACE - add extra (random) perturbations;
4. Q - start/stop wireframe visualization;
5. F5 - start/stop the simulation;
5. ESC - close window.

**Extra features implemented on the scene:**
1. Using textures (pool tiles);
2. advanced camera controlling;
3. interactivity (you may add perturbations using SPACE key);

### Run

Solution to the task is written on C++: [main.cpp](main.cpp). 

**Requirements:** \
you need to have **OpenGL3**, **glfw3**, **glew**, **X11**, **SOIL**, **glm** libraries installed.

To install them do: \
**`$ sudo apt update`** \
**`$ sudo apt install libglfw3-dev`** \
**`$ sudo apt install libglew-dev`** \
**`$ sudo apt install libglm-dev`** \
**`$ sudo apt install libx11-dev`** \
**`$ sudo apt install libsoil-dev`**

For more information about libraries used: [CMakeLists.txt](CMakeLists.txt).

To compile the program do: \
**`$ cmake .`** \
**`$ make -j 4`**. 

To run the program do: \
**`$ ./WaterSim`**.
