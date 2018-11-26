//
// Created by zoe on 03/10/17.
//
#include <fstream>
#include <string>
#include <vector>
#include "glfw.h"


namespace glfw
{
    GLFWwindow* window;

    int runWindow(int resX, int resY, bool windowed)
    {
        // Initialise GLFW
        if (!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW\n");
            getchar();
            return -1;
        }


		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(resX, resY, "Vulkan window", nullptr, nullptr);

		// create call back gfor resize
		//glfwSetWindowUserPointer(window, Renderer::get());
		//glfwSetWindowSizeCallback(window, Renderer::onWindowResized);


        return 0;

    }

	void closeWindow()
	{

	}
	
}
