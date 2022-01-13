#ifndef PATHTRACER_MAIN_H
#define PATHTRACER_MAIN_H

#include "gfx/Window.h"
#include "Camera.h"

class GameWindow : public Window {
public:
    Camera camera = *new Camera(glm::vec3(0), glm::vec3(0, 1, 0));

    GameWindow()
    : Window("Game Frame") {

    }

    void OnLoad() override {

    }

    void OnClose() override {

    }

    void OnUpdateFrame() override {
        //Child implementation
        Window::OnUpdateFrame();
    }

    void OnRenderFrame() override {
        Window::OnRenderFrame();
        //Child implementation
    }

    void OnGameTick() override {
        Window::OnGameTick();
        //Child implementation
    }

    void OnResize() override {

    }

    void OnKeyPress() override {

    }
};

#endif //PATHTRACER_MAIN_H
