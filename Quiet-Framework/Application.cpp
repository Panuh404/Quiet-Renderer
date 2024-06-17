#include "Application.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <ratio>

Application::Application(const std::string& title) : _title(title) {}

Application::~Application()
{
    Cleanup();
}

bool Application::Initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW: Unable to initialize\n";
        return false;
    }

    // Get Primary Monitor and Resolution
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    _width = static_cast<int32_t>(videoMode->width * 0.8f);
    _height = static_cast<int32_t>(videoMode->height * 0.8f);

    // More in (https://www.glfw.org/docs/3.3/group__window.html)
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);// No scale    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);// No create context for OpenGL

    // Create Window
    _window = glfwCreateWindow(_width, _height, _title.data(), nullptr, nullptr);
    if (_window == nullptr)
    {
        std::cerr << "GLFW: Unable to create window\n";
        Cleanup();
        return false;
    }

    // Set windows dimension and position
    const int32_t windowLeft = videoMode->width / 2 - _width / 2;
    const int32_t windowTop = videoMode->height / 2 - _height / 2;
    glfwSetWindowPos(_window, windowLeft, windowTop);

    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, HandleResize);

    _currentTime = std::chrono::high_resolution_clock::now();
    return true;
}

void Application::OnResize(const int32_t width, const int32_t height)
{
    _width = width;
    _height = height;
}

void Application::Cleanup()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Application::Run()
{
    if (!Initialize())
    {
        return;
    }
    if (!Load())
    {
        return;
    }
    while (!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
        Update();
        Render();
    }
}

void Application::HandleResize(GLFWwindow* window, const int32_t width, const int32_t height)
{
    Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->OnResize(width, height);
}

void Application::Update()
{
    auto oldTime = _currentTime;
    _currentTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> timeSpan = (_currentTime - oldTime);
    _deltaTime = static_cast<float>(timeSpan.count() / 1000.0);
    glfwPollEvents();
}



