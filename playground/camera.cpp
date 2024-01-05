#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch,
           const glm::vec3& cubeCenterParam, float radiusParam)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM),
      Position(position),
      WorldUp(up),
      Yaw(yaw),
      Pitch(pitch),
      cubeCenter(cubeCenterParam), // Initialize with default value
      radius(radiusParam) {
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Calculate the camera's position using spherical coordinates
    glm::vec3 newPosition;
    newPosition.x = cubeCenter.x + radius * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    newPosition.y = cubeCenter.y + radius * sin(glm::radians(Pitch));
    newPosition.z = cubeCenter.z + radius * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));

    // Update the camera's position
    Position = newPosition;

    // Calculate the new Front vector as pointing from the camera to the cube center
    Front = glm::normalize(cubeCenter - Position);

    // Conditionally determine the Up vector based on the inverted camera angle
    if (Pitch > 89.0f || Pitch < -89.0f) {
        // If we have flipped over the top or bottom, the Up vector should be inverted
        Up = glm::vec3(0.0f, -1.0f, 0.0f);
    } else {
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // Recalculate Right vector
    Right = glm::normalize(glm::cross(Front, Up));
}


// Camera class method to handle keyboard input for rotation
void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    // Increase this value for faster rotation (degrees per second)
    float rotationSpeed = 360.0f; // You can adjust this value as needed
    float angleChange = rotationSpeed * deltaTime;

    // Now apply this updated angle change based on deltaTime and direction
    if (direction == FORWARD)
        Pitch -= angleChange; // Example: look up
    else if (direction == BACKWARD)
        Pitch += angleChange; // Example: look down
    else if (direction == LEFT)
        Yaw -= angleChange; // Example: rotate left
    else if (direction == RIGHT)
        Yaw += angleChange; // Example: rotate right

    // Ensure Yaw wraps around at 360 degrees
    if (Yaw > 360.0f)
        Yaw -= 360.0f;
    else if (Yaw < 0.0f)
        Yaw += 360.0f;

    // Ensure Pitch is clamped to prevent flipping
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    else if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors(); // Update the camera's direction vectors based on the new yaw and pitch
}
// Implement the Camera::updateProjectionMatrix method, presumably inside Camera.cpp
void Camera::updateProjectionMatrix(float fov, float width, float height, float nearPlane, float farPlane) {
    projectionMatrix = glm::perspective(glm::radians(fov), width / height, nearPlane, farPlane);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}