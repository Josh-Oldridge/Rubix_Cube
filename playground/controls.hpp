#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "Camera.hpp"
#include <glfw3.h>


class Controls {
public:
    Controls(GLFWwindow* window, Camera* camera);

    void update(float deltaTime); 

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


private:
    GLFWwindow* window;
    Camera* camera;

    // Mouse state
    bool rightButtonPressed;
    double lastX, lastY;
    float deltaTime;

    
};

#endif // CONTROLS_HPP