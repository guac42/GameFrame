#include "Camera.h"
#include "Main.h"

void Camera::processInputs(float dT, bool& frameChanged) {
    frameChanged = false;

    glm::vec2 mouseDelta = game.mouseManager.delta;
    yaw += mouseDelta.x * MouseSensitivity;
    pitch -= mouseDelta.y * MouseSensitivity;

    if (pitch >= 90) pitch = 89.999f;
    if (pitch <= -90) pitch = -89.999f;

    Front.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
    Front.y = glm::sin(glm::radians(this->pitch));
    Front.z = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));

    glm::vec3 acceleration =
            game.keyboardManager.isKeyDown(GLFW_KEY_W) * Front -
            game.keyboardManager.isKeyDown(GLFW_KEY_S) * Front +
            game.keyboardManager.isKeyDown(GLFW_KEY_D) * glm::normalize(glm::cross(Front, Up)) -
            game.keyboardManager.isKeyDown(GLFW_KEY_A) * glm::normalize(glm::cross(Front, Up));
    Velocity +=
            game.keyboardManager.isKeyDown(GLFW_KEY_LEFT_SHIFT) ? acceleration * 5.0f : (
            game.keyboardManager.isKeyDown(GLFW_KEY_LEFT_CONTROL) ? acceleration * 0.35f : acceleration);

    frameChanged = mouseDelta.x != 0 || mouseDelta.y != 0 || acceleration != glm::vec3(0) || Velocity != glm::vec3(0);

    // Velocity cut off
    Velocity *= (float)(glm::dot(Velocity, Velocity) >= 0.01f) * 0.95f;
    Velocity += acceleration * dT;
    Position += Velocity * dT;
    this->view = glm::lookAt(Position, Position + Front, Up);
}