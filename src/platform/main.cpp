#include <iostream>
#include "engine.h"

int main() {

	auto app = engine::get();

	app->add_subsystem("vulkan", vulkan_platform::get());

	try {
		app->run();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
