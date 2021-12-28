#include "Window.h"
#include "../Main.h"

// Internal callback
void Window::_key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) {
    if (key < 0) return;
    switch (key) {
        // Fullscreen
        case GLFW_KEY_F:
            if (action == GLFW_PRESS)
                if (glfwGetWindowAttrib(handle, GLFW_MAXIMIZED))
                    glfwRestoreWindow(handle);
                else
                    glfwMaximizeWindow(handle);
                break;
                // Exit
                case GLFW_KEY_ESCAPE:
                    if (action == GLFW_PRESS)
                        glfwSetWindowShouldClose(handle, GLFW_TRUE);
                    break;
                    // Lock cursor
                    case GLFW_KEY_T:
                        if (action == GLFW_PRESS)
                            glfwSetInputMode(handle, GLFW_CURSOR,
                                             glfwGetInputMode(handle, GLFW_CURSOR) == GLFW_CURSOR_NORMAL
                                             ? GLFW_CURSOR_DISABLED
                                             : GLFW_CURSOR_NORMAL);
                        break;
                        default:
                            break;
    }
    // Key states
    game.keyboardManager.update(key, action);
    game.OnKeyPress();
}

void Window::_cursor_callback(GLFWwindow *handle, double xp, double yp) {
    glm::vec2 p{xp, yp};
    game.mouseManager.delta.y = game.mouseManager.position.y - p.y;
    game.mouseManager.delta.x = p.x - game.mouseManager.position.x;
    game.mouseManager.delta = glm::clamp(game.mouseManager.delta, -100.0f, 100.0f);
    game.mouseManager.position = p;
}

void Window::_mouse_callback(GLFWwindow *handle, int button, int action, int mods) {
    if (button < 0) return;
    game.mouseManager.update(button, action);
}

void Window::_size_callback(GLFWwindow *handle, int width, int height) {
    glViewport(0, 0, width, height);
    game.width = width;
    game.height = height;
    game.OnResize();
}

// Base callbacks
void Window::OnUpdateFrame() {
    game.mouseManager.delta = glm::vec2{0.0, 0.0};
}

void Window::OnRenderFrame() {
    game.frames++;
}

void Window::OnGameTick() {
    game.ticks++;
}

// Main Window loop
void Window::Run() {
    game.OnLoad();

    while (!glfwWindowShouldClose(game.handle)) {
        const unsigned long long start = NOW();

        game.frame_delta = start - game.last_frame;
        game.last_frame = start;

        if (start - game.last_second > NS_PER_SECOND) {
            game.fps = game.frames;
            game.tps = game.ticks;
            game.frames = 0;
            game.ticks = 0;
            game.last_second = start;

            printf("FPS: %lld | TPS: %lld\n", game.fps, game.tps);
        }

        // tick processing
        const unsigned long long NS_PER_TICK = (NS_PER_SECOND / TICKS_PER_SECOND);
        unsigned long long tick_time = game.frame_delta + game.tick_remainder;
        while (tick_time > NS_PER_TICK) {
            game.OnGameTick();
            tick_time -= NS_PER_TICK;
        }
        game.tick_remainder = std::max(tick_time, (unsigned long long) 0);

        game.OnUpdateFrame();
        game.OnRenderFrame();
        glfwSwapBuffers(game.handle);
        glfwPollEvents();

        //DELAY(NS_PER_TICK - (start - NOW()));
    }

    game.OnClose();
    glfwTerminate();
    exit(0);
}