#include <iostream>
#include "engine.h"
#include "engine_state.h"
#include "engine_state_machine.h"
#include "entity_manager.h"
#include "camera_system.h"

int main() {

	auto app = engine::get();

	app->add_subsystem("renderer", vulkan_platform::get());
	app->add_subsystem("state_machine", engine_state_machine::get());
	app->add_subsystem("entity_manager", entity_manager::get());
	app->add_subsystem("camera_system", camera_system::get());

	engine_state_machine::get()->add_state("start_state", std::make_shared<engine_state_1>(), state_type::START);
	engine_state_machine::get()->change_state("start_state");

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
