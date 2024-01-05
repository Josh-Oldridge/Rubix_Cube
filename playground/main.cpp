#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw3.h>
#include "cube.hpp"
#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>

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
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Rubik's Cube", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, 800, 600);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Define the view matrix to position and orient the camera
        glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is here in world space
        glm::vec3(0, 0, 0), // and looks here: at the origin
        glm::vec3(0, 1, 0)  // Head is up
    );

// Define the projection matrix for a perspective view
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Build and compile our shader program (from shader.hpp)
    // Load shaders and create a shader program
    GLuint shaderProgram = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    // Setup the cube
    setupCube();

    // Generate the cubies for the Rubik's cube
    std::vector<Cubie> rubiksCubeCubies = generateCubies();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the color and depth buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program when drawing
        glUseProgram(shaderProgram);


        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (const Cubie& cubie : rubiksCubeCubies) {
            drawCube(shaderProgram, cubie);
        }
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);  // Delete the shader program when done

    // Properly de-allocate all resources once they've outlived their purpose
    cleanupCube();

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return 0;
}