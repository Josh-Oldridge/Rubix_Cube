#include "controls.hpp"

#include "Controls.hpp"

// Constructor for the Controls class initializes member variables and sets up GLFW callbacks
Controls::Controls(GLFWwindow* window, Camera* camera) 
    : window(window), camera(camera), rightButtonPressed(false), lastX(0.0), lastY(0.0) {
    glfwSetKeyCallback(window, Controls::keyCallback);
    glfwSetMouseButtonCallback(window, Controls::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Controls::cursorPositionCallback);
    glfwSetScrollCallback(window, Controls::scrollCallback);

    // Get the cursor position
    glfwGetCursorPos(window, &lastX, &lastY);
}

// Callback to handle key press events
void Controls::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_W:
                controls->camera->processKeyboard(FORWARD, controls->deltaTime);
                break;
            case GLFW_KEY_S:
                controls->camera->processKeyboard(BACKWARD, controls->deltaTime);
                break;
            case GLFW_KEY_A:
                controls->camera->processKeyboard(LEFT, controls->deltaTime);
                break;
            case GLFW_KEY_D:
                controls->camera->processKeyboard(RIGHT, controls->deltaTime);
                break;
            // Add more key cases as needed
        }
    }
}

// Callback to handle mouse button events
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

// Callback to handle cursor position events
void Controls::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    
    if (controls->rightButtonPressed) {
        float xoffset = xpos - controls->lastX;
        float yoffset = controls->lastY - ypos; // Reversed since y-coordinates go from bottom to top

        // Process the camera's mouse movement
        controls->camera->processMouseMovement(xoffset, yoffset);
    }

    controls->lastX = xpos;
    controls->lastY = ypos;
}

// Callback to handle the scroll events
void Controls::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Controls* controls = static_cast<Controls*>(glfwGetWindowUserPointer(window));
    controls->camera->processMouseScroll(yoffset);
}

void Controls::update(float deltaTime) {
    // Check for WASD key presses and update the camera position accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, deltaTime);

    // Optionally, you could handle other continuous inputs here,
    // such as mouse dragging for looking around
}