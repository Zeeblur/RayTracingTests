//
// Created by zoe on 01/10/17.
//
#pragma once

#include <stdlib.h>
#include <stdio.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>

#include <iostream>
#include <ios>

namespace glfw
{
    extern GLFWwindow* window;
    int runWindow(int resX, int resY, bool windowed);
	void closeWindow();
}
