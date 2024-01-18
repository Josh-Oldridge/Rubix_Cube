#include "controls.hpp"

#include "Controls.hpp"

Controls::Controls(GLFWwindow* window, Camera* camera) 
    : window(window), camera(camera), rightButtonPressed(false), lastX(0.0), lastY(0.0) {
    glfwSetKeyCallback(window, Controls::keyCallback);
    glfwSetMouseButtonCallback(window, Controls::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Controls::cursorPositionCallback);
    glfwGetCursorPos(window, &lastX, &lastY);
}

void Controls::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
}

void Controls::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            controls->rightButtonPressed = true;
        } else if (action == GLFW_RELEASE) {
            controls->rightButtonPressed = false;
        }
    }
}

void Controls::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    
    if (controls->rightButtonPressed) {
        float xoffset = xpos - controls->lastX;
        float yoffset = ypos - controls->lastY; 
        controls->camera->processMouseMovement(xoffset, yoffset);
    }

    controls->lastX = xpos;
    controls->lastY = ypos;
}

void Controls::update(float deltaTime) {
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, deltaTime);
}

void Controls::setDeltaTime(float dt) {
    deltaTime = dt;
}