#pragma once

#include <cstdint>
#include <string>
#include <chrono>

struct GLFWwindow;

class Application
{
public:
    Application(const std::string& title);
    virtual ~Application();
    void Run();

protected:
    static void HandleResize(GLFWwindow* window, int32_t width, int32_t height);
    virtual void OnResize(int32_t width, int32_t height);

    virtual bool Initialize();
    virtual bool Load() = 0;
    virtual void Cleanup();
    virtual void Render() = 0;
    virtual void Update();

    [[nodiscard]] inline GLFWwindow* GetWindow() const { return _window; }
    [[nodiscard]] inline int32_t GetWindowWidth() const { return _width; }
    [[nodiscard]] inline int32_t GetWindowHeight() const { return _height; }

    int32_t _width = 0;
    int32_t _height = 0;
    float _deltaTime = 0.016f;

private:
    std::chrono::high_resolution_clock::time_point _currentTime;
    GLFWwindow* _window = nullptr;
    std::string_view _title;
};

