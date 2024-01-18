#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch,
           const glm::vec3& cubeCenterParam, float radiusParam);

    glm::mat4 GetViewMatrix() const;
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processKeyboard(Camera_Movement direction, float deltaTime);
    void updateProjectionMatrix(float fov, float width, float height, float nearPlane, float farPlane);
    glm::mat4 getProjectionMatrix() const;

private:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 cubeCenter;
    glm::mat4 projectionMatrix;
    
    float Yaw;
    float Pitch;
    float radius; 
    
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
    void updateCameraVectors();
};

#endif // CAMERA_HPP