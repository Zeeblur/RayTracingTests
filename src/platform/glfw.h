#pragma once

#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_VULKAN
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <iostream>
#include <ios>

namespace glfw
{
    extern GLFWwindow* window;
    int runWindow(int resX, int resY);
	void closeWindow();
}
