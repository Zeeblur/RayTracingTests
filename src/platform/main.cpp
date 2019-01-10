#include <iostream>
#include "engine.h"
#include "game_state.h"
#include "engine_state_machine.h"
#include "entity_manager.h"
#include "camera_system.h"
#include "physics_system.h"

int main() {

	auto app = engine::get();

	app->add_subsystem("entity_manager", entity_manager::get());
	app->add_subsystem("renderer", vulkan_platform::get());
	app->add_subsystem("state_machine", engine_state_machine::get());
	app->add_subsystem("camera_system", camera_system::get());
	app->add_subsystem("input_handler", input_handler::get());
	app->add_subsystem("physics_system", physics_system::get());

	engine_state_machine::get()->add_state("game_state", std::make_shared<engine_state_1>(), state_type::START);
	engine_state_machine::get()->change_state("game_state");

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
