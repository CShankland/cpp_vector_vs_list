//
// Created by chris on 7/18/2020.
//

#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#define GL_CHECK(fn) fn;GetGLError(__FILE__, __LINE__)
static void GetGLError(const char* file, int line) {
    GLenum error = 0;
    while ((error = glGetError())) {
        const char* glErrorStr = nullptr;
        switch (error) {
            case GL_INVALID_ENUM:
                glErrorStr = "INVALID ENUM";
                break;
            case GL_INVALID_VALUE:
                glErrorStr = "INVALID VALUE";
                break;
            case GL_INVALID_OPERATION:
                glErrorStr = "INVALID OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                glErrorStr = "INVALID FRAMEBUFFER OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                glErrorStr = "OUT OF MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                glErrorStr = "STACK UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                glErrorStr = "STACK OVERFLOW";
                break;
        }

        printf("OGL error %d (%s) in %s at line %d\n", error, glErrorStr, file, line);
    }
}

static Window* gInstance = nullptr;

static void
glfwErrorCallback(int error, const char* description)
{
    printf("GLFW Error: (%d) - %s\n", error, description);
}

static void
glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

bool Window::init(char const* title, int width, int height)
{
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        return false;
    }

    gInstance = this;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, glfwKeyCallback);

    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); // Glew leaves an error

    // Version info
    const GLubyte* renderer = GL_CHECK(glGetString(GL_RENDERER));
    const GLubyte* version = GL_CHECK(glGetString(GL_VERSION));

    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported: %s\n", version);

    // Enable depth testing
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glDepthFunc(GL_LESS));

    return true;
}

void Window::term()
{
    if (window != nullptr)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    gInstance = nullptr;
    glfwTerminate();
}

bool Window::beginFrame()
{
    if (nullptr == window)
    {
        return false;
    }

    if (glfwWindowShouldClose(window))
    {
        return false;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    return true;
}

void Window::endFrame()
{
    glfwSwapBuffers(window);
}
