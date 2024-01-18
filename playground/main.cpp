#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw3.h>
#include "cube.hpp"
#include "camera.hpp"
#include "controls.hpp"
#include "rubiksCube.hpp"
#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main() {

    
    // Initialize GLFW and create a window
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For macOS
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Rubik's Cube", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    


    // Instantiate Camera object and set it as the user pointer for GLFW
    Camera camera(glm::vec3(0, 0, 10), glm::vec3(0, 1, 0), -90.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);  
    // Set the user pointer for the window to the camera

    // Retrieve initial window size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // Add this line to get the current window size

    // Update the camera with the initial projection matrix based on the window size
    camera.updateProjectionMatrix(45.0f, static_cast<float>(width), static_cast<float>(height), 0.1f, 100.0f);

    glfwSetWindowUserPointer(window, &camera);

    // After creating the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, 1920, 1080);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // Specify the depth test to use
    glDisable(GL_BLEND);

    // Define the projection matrix for a perspective view
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);


    // Instantiate Controls object and hook it into GLFW
    Controls controls(window, &camera);
    glfwSetWindowUserPointer(window, &controls);

    // Set the GLFW callbacks for mouse and keyboard
    glfwSetKeyCallback(window, Controls::keyCallback);
    glfwSetMouseButtonCallback(window, Controls::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Controls::cursorPositionCallback);
    // Build and compile our shader program (from shader.hpp)
    // Load shaders and create a shader program
    GLuint shaderProgram = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    // Generate cubies for the Rubik's cube and setup their VAOs and VBOs
    std::vector<Cubie> rubiksCubeCubies = generateRubiksCubeCubies();

    // Variables for deltaTime calculation
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        controls.setDeltaTime(deltaTime);

        // Update controls
        controls.update(deltaTime);

        // Clear the color and depth buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program when drawing
        glUseProgram(shaderProgram);

        // Get the updated view matrix from the Camera object
        glm::mat4 view = camera.GetViewMatrix();

        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection = camera.getProjectionMatrix();
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (const Cubie& cubie : rubiksCubeCubies) {
            drawCubie(shaderProgram, cubie);
        }
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

    }

    // Cleanup
    cleanupCubies(rubiksCubeCubies);  // Call the new cleanup function
    glDeleteProgram(shaderProgram);    // Delete the shader program

    // Terminate GLFW to clear any allocated resources
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0) return; // Skip processing if size is 0
    glViewport(0, 0, width, height);
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->updateProjectionMatrix(glm::radians(45.0f), width, height, 0.1f, 100.0f);
    }
}

