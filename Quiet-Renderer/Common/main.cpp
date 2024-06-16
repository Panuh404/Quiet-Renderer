#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

//////////////////////////////////////////////////////////////////
//	THIS IS TEST ONLY PROJECT									//
//	THE FINAL IMPLEMENTATION OF THE RENDERER WILL BE IN QUIET	//
//////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
    // Initialize GLFW
	if (!glfwInit())
    {
        std::cout << "GLFW: Unable to initialize\n";
        return -1;
    }

    // Get Primary Monitor and Resolution
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    const int32_t width = static_cast<int32_t>(videoMode->width * 0.9f);
    const int32_t height = static_cast<int32_t>(videoMode->height * 0.9f);

    // No scale
    // More in (https://www.glfw.org/docs/3.3/group__window.html)
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

    // Create Window
    GLFWwindow* window = glfwCreateWindow(width, height, "Quiet-Renderer", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "GLFW: Unable to create window\n";
        glfwTerminate();
        return -1;
    }

    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // future update code
        // future render code
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

