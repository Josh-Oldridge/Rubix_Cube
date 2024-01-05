#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    GLfloat position[3];
    GLfloat color[3];
};

struct Cubie {
    glm::vec3 position;
    std::vector<Vertex> vertices;
    GLuint VAO; // Add this
    GLuint VBO; // Add this
};

void setupCubieBuffers(Cubie& cubie);
void drawCubie(GLuint shaderProgram, const Cubie& cubie);
std::vector<Cubie> generateRubiksCubeCubies(); 
std::vector<Vertex> createCubieVertices(const glm::vec3 (&colors)[6]);
void cleanupCubies(std::vector<Cubie>& rubiksCubeCubies);


#endif // CUBE_HPP