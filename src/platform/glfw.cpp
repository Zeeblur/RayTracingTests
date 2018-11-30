#include <fstream>
#include <string>
#include <vector>
#include "glfw.h"

namespace glfw
{
    GLFWwindow* window;

    int runWindow(int resX, int resY)
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
		//glfwSetWindowUserPointer(window, vulkan_platform::get());
		//glfwSetWindowSizeCallback(window, vulkan_platform::onWindowResized);


        return 0;

    }

	void closeWindow()
	{

	}
	
}
