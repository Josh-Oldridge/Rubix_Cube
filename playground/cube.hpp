#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

// Cubie structure to hold state (position for now)
struct Cubie {
    glm::vec3 position;
    // ... later you might add orientation or color information ...
};

void setupCube();
void drawCube(GLuint shaderProgram, Cubie cubie);
void cleanupCube();
std::vector<Cubie> generateCubies(); 


#endif // CUBE_HPP