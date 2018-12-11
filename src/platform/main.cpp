#include <iostream>
#include "vulkan_platform.h"

int main() {

	vulkan_platform app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
