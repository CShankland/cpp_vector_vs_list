#pragma once

//
// Created by chris on 7/18/2020.
//

struct GLFWwindow;

class Window {
public:
    Window(): window(nullptr) {}

    bool init(char const* title, int width, int height);
    void term();

    bool beginFrame();
    void endFrame();
private:
    GLFWwindow* window;
};
