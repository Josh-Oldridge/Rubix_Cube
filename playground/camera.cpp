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
      cubeCenter(cubeCenterParam),
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
    
    glm::vec3 newPosition;
    newPosition.x = cubeCenter.x + radius * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    newPosition.y = cubeCenter.y + radius * sin(glm::radians(Pitch));
    newPosition.z = cubeCenter.z + radius * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    Position = newPosition;
    Front = glm::normalize(cubeCenter - Position);

    if (Pitch > 89.0f || Pitch < -89.0f) {
        Up = glm::vec3(0.0f, -1.0f, 0.0f);
    } else {
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    Right = glm::normalize(glm::cross(Front, Up));
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    
    float rotationSpeed = 360.0f;
    float angleChange = rotationSpeed * deltaTime;

    if (direction == FORWARD)
        Pitch -= angleChange;
    else if (direction == BACKWARD)
        Pitch += angleChange;
    else if (direction == LEFT)
        Yaw -= angleChange;
    else if (direction == RIGHT)
        Yaw += angleChange;

    if (Yaw > 360.0f)
        Yaw -= 360.0f;
    else if (Yaw < 0.0f)
        Yaw += 360.0f;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    else if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors(); 
}

void Camera::updateProjectionMatrix(float fov, float width, float height, float nearPlane, float farPlane) {
    projectionMatrix = glm::perspective(glm::radians(fov), width / height, nearPlane, farPlane);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}