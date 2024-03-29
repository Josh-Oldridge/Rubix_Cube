#include "cube.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

GLuint VAO, VBO, EBO;
std::vector<Vertex> createCubieVertices(const glm::vec3 (&colors)[6]) {

    std::vector<Vertex> vertices;
    vertices.reserve(36);

    const GLfloat vertexPositions[8][3] = {
        {-0.5f, -0.5f, -0.5f}, // Back-bottom-left
        { 0.5f, -0.5f, -0.5f}, // Back-bottom-right
        { 0.5f,  0.5f, -0.5f}, // Back-top-right
        {-0.5f,  0.5f, -0.5f}, // Back-top-left
        {-0.5f, -0.5f,  0.5f}, // Front-bottom-left
        { 0.5f, -0.5f,  0.5f}, // Front-bottom-right
        { 0.5f,  0.5f,  0.5f}, // Front-top-right
        {-0.5f,  0.5f,  0.5f}, // Front-top-left
    };

    const GLuint faceIndices[6][4] = {
        // Front face (when looking from the 0.5f on the z-axis towards the origin)
        {4, 7, 6, 5},
        // Back face (when looking from the -0.5f on the z-axis towards the origin)
        {0, 3, 2, 1},
        // Left face (when looking from the -0.5f on the x-axis towards the origin)
        {4, 0, 3, 7},
        // Right face (when looking from the 0.5f on the x-axis towards the origin)
        {1, 5, 6, 2},
        // Top face (when looking from the 0.5f on the y-axis towards the origin)
        {7, 3, 2, 6},
        // Bottom face (when looking from the -0.5f on the y-axis towards the origin)
        {0, 4, 5, 1},
    };
    
    for (int face = 0; face < 6; face++) {

        vertices.push_back(Vertex{{vertexPositions[faceIndices[face][0]][0], vertexPositions[faceIndices[face][0]][1], vertexPositions[faceIndices[face][0]][2]}, {colors[face].x, colors[face].y, colors[face].z}});
        vertices.push_back(Vertex{{vertexPositions[faceIndices[face][1]][0], vertexPositions[faceIndices[face][1]][1], vertexPositions[faceIndices[face][1]][2]}, {colors[face].x, colors[face].y, colors[face].z}});
        vertices.push_back(Vertex{{vertexPositions[faceIndices[face][2]][0], vertexPositions[faceIndices[face][2]][1], vertexPositions[faceIndices[face][2]][2]}, {colors[face].x, colors[face].y, colors[face].z}});

        vertices.push_back(Vertex{{vertexPositions[faceIndices[face][2]][0], vertexPositions[faceIndices[face][2]][1], vertexPositions[faceIndices[face][2]][2]}, {colors[face].x, colors[face].y, colors[face].z}});
        vertices.push_back(Vertex{{vertexPositions[faceIndices[face][3]][0], vertexPositions[faceIndices[face][3]][1], vertexPositions[faceIndices[face][3]][2]}, {colors[face].x, colors[face].y, colors[face].z}});
        vertices.push_back(Vertex{{vertexPositions[faceIndices[face][0]][0], vertexPositions[faceIndices[face][0]][1], vertexPositions[faceIndices[face][0]][2]}, {colors[face].x, colors[face].y, colors[face].z}});
    }

    return vertices;
}

std::vector<Cubie> generateRubiksCubeCubies() {
    std::vector<Cubie> rubiksCubeCubies;
    glm::vec3 colorRed = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 colorOrange = glm::vec3(1.0f, 0.65f, 0.0f);
    glm::vec3 colorGreen = glm::vec3(0.0f, 0.5f, 0.0f);
    glm::vec3 colorBlue = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 colorWhite = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 colorYellow = glm::vec3(1.0f, 1.0f, 0.0f);
    glm::vec3 colorBlack = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                glm::vec3 cubieColor[6] = {
                    (z == 2) ? colorRed : colorBlack,
                    (z == 0) ? colorOrange : colorBlack,
                    (x == 0) ? colorGreen : colorBlack, 
                    (x == 2) ? colorBlue : colorBlack, 
                    (y == 2) ? colorWhite : colorBlack,
                    (y == 0) ? colorYellow : colorBlack,
                };
                std::vector<Vertex> cubieVertices = createCubieVertices(cubieColor);
                glm::vec3 position((x - 1), (y - 1), (z - 1));
                rubiksCubeCubies.push_back(Cubie{position, cubieVertices});
            }
        }
    }

    for (Cubie& cubie : rubiksCubeCubies) {
        setupCubieBuffers(cubie);
    }

    return rubiksCubeCubies;
}

void setupCubieBuffers(Cubie& cubie) {
    glGenVertexArrays(1, &cubie.VAO);
    glGenBuffers(1, &cubie.VBO);
    glBindVertexArray(cubie.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubie.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cubie.vertices.size(), cubie.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0); 
}

void drawCubie(GLuint shaderProgram, const Cubie& cubie) {

    glm::mat4 model = glm::translate(glm::mat4(1.0f), cubie.position);
    model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.95f));
    
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(cubie.VAO);
    glDrawArrays(GL_TRIANGLES, 0, cubie.vertices.size());
    glBindVertexArray(0);
}

void cleanupCubies(std::vector<Cubie>& rubiksCubeCubies) {
    for (Cubie& cubie : rubiksCubeCubies) {
        glDeleteVertexArrays(1, &cubie.VAO);
        glDeleteBuffers(1, &cubie.VBO);
    }
}