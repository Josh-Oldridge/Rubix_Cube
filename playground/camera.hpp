#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch);

    glm::mat4 GetViewMatrix() const;
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);
    void processKeyboard(Camera_Movement direction, float deltaTime);

    // TODO: Add other necessary methods and member variables

private:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    // Euler Angles
    float Yaw;
    float Pitch;
    
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
    void updateCameraVectors();
};

#endif // CAMERA_HPP