#ifndef PATHTRACER_MAIN_H
#define PATHTRACER_MAIN_H

#include "gfx/Window.h"
#include "Camera.h"

class GameWindow : public Window {
public:
    Camera camera = *new Camera(glm::vec3(0), glm::vec3(0, 1, 0));

    GameWindow()
    : Window(832, 468, "Path Tracer") {

    }

    void OnLoad() override {

    }

    void OnClose() override {

    }

    void OnUpdateFrame() override {


        Window::OnUpdateFrame();
    }

    void OnRenderFrame() override {
        Window::OnRenderFrame();

    }

    void OnGameTick() override {
        Window::OnGameTick();
    }

    void OnResize() override {

    }

    void OnKeyPress() override {

    }
};

extern GameWindow game;

#endif //PATHTRACER_MAIN_H
