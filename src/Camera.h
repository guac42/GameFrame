#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gfx/Window.h"

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

    void processInputs(Window *game, float dT, bool& frameChanged) {
        frameChanged = false;

        glm::vec2 mouseDelta = game->mouseManager.delta;
        yaw += mouseDelta.x * MouseSensitivity;
        pitch -= mouseDelta.y * MouseSensitivity;

        if (pitch >= 90) pitch = 89.999f;
        if (pitch <= -90) pitch = -89.999f;

        Front.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
        Front.y = glm::sin(glm::radians(this->pitch));
        Front.z = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));

        glm::vec3 acceleration =
                (float)game->keyboardManager.isKeyDown(GLFW_KEY_W) * Front -
                (float)game->keyboardManager.isKeyDown(GLFW_KEY_S) * Front +
                (float)game->keyboardManager.isKeyDown(GLFW_KEY_D) * glm::normalize(glm::cross(Front, Up)) -
                (float)game->keyboardManager.isKeyDown(GLFW_KEY_A) * glm::normalize(glm::cross(Front, Up));
        Velocity +=
                game->keyboardManager.isKeyDown(GLFW_KEY_LEFT_SHIFT) ? acceleration * 5.0f : (
                        game->keyboardManager.isKeyDown(GLFW_KEY_LEFT_CONTROL) ? acceleration * 0.35f : acceleration);

        frameChanged = mouseDelta.x != 0 || mouseDelta.y != 0 || acceleration != glm::vec3(0) || Velocity != glm::vec3(0);

        // Velocity cut off
        Velocity *= (float)(glm::dot(Velocity, Velocity) >= 0.01f) * 0.95f;
        Velocity += acceleration * dT;
        Position += Velocity * dT;
        this->view = glm::lookAt(Position, Position + Front, Up);
    }

private:
    float yaw, pitch;
    glm::mat4 view;


};

#endif //PATHTRACER_CAMERA_H
