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

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Rubik's Cube", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    Camera camera(glm::vec3(0, 0, 10), glm::vec3(0, 1, 0), -90.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);  
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    camera.updateProjectionMatrix(45.0f, static_cast<float>(width), static_cast<float>(height), 0.1f, 100.0f);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    glViewport(0, 0, 1920, 1080);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    Controls controls(window, &camera);
    glfwSetWindowUserPointer(window, &controls);
    glfwSetKeyCallback(window, Controls::keyCallback);
    glfwSetMouseButtonCallback(window, Controls::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Controls::cursorPositionCallback);
    GLuint shaderProgram = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    std::vector<Cubie> rubiksCubeCubies = generateRubiksCubeCubies();
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    

 
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        controls.setDeltaTime(deltaTime);
        controls.update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glm::mat4 view = camera.GetViewMatrix();
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 projection = camera.getProjectionMatrix();
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (const Cubie& cubie : rubiksCubeCubies) {
            drawCubie(shaderProgram, cubie);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    cleanupCubies(rubiksCubeCubies);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0) return; 
    glViewport(0, 0, width, height);
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->updateProjectionMatrix(glm::radians(45.0f), width, height, 0.1f, 100.0f);
    }
}

