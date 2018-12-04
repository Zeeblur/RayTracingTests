#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "engine.h"
#include "state_machine.h"
#include "engine_state_machine.h"
#include "start_state.h"
#include "thread_pool.h"

// Include GLFW
#include <GLFW/glfw3.h>

using namespace std;
// Include GLM
#include <glm/glm.hpp>
using namespace glm;

int main()
{
	std::cout << "hello world" << std::endl;

	auto eng = engine::get();
	eng->initWindowMan();
	eng->add_subsystem("vulkan", vulkan_platform::get());
	//eng->add_subsystem("entity_manager", entity_manager::get());
	//eng->add_subsystem("physics_system", physics_system::get());
	//eng->add_subsystem("renderer", renderer::get());
	//eng->add_subsystem("state_machine", engine_state_machine::get());
	//eng->add_subsystem("input_handler", input_handler::get());
	//eng->add_subsystem("camera_system", camera_system::get());

	engine_state_machine::get()->add_state("start_state", std::make_shared<start_state>(), state_type::START);
	engine_state_machine::get()->change_state("start_state");
	eng->run();

	return 0;
}