#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position, Front, Up, Velocity;
    float MovementSpeed, MouseSensitivity;

    Camera(const glm::vec3& position, const glm::vec3& up, float yaw = -90.0f,
           float pitch = 0.0f, float mouseSensitivity = 0.1f, float speed = 10.0f) {
        this->yaw = yaw;
        this->pitch = pitch;

        Front.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
        Front.y = glm::sin(glm::radians(this->pitch));
        Front.z = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));

        this->view = glm::lookAt(position, position + Front, up);
        Position = position;
        Up = up;
        MovementSpeed = speed;
        MouseSensitivity = mouseSensitivity;
    }

    inline glm::mat4 getView() const {
        return this->view;
    }

    inline float getYaw() const {
        return this->yaw;
    }

    inline float getPitch() const {
        return this->pitch;
    }

    void processInputs(float dT, bool& frameChanged);

private:
    float yaw, pitch;
    glm::mat4 view;


};

#endif //PATHTRACER_CAMERA_H
