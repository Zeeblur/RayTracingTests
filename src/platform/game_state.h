#pragma once
#include "engine_state.h"

// Engine state 1 will be first simulation
class engine_state_1 : public engine_state
{
public:
	void on_enter()
	{
		// Turn on the systems
		engine::get()->get_subsystem("entity_manager")->set_active(true);
		engine::get()->get_subsystem("entity_manager")->set_visible(true);
		engine::get()->get_subsystem("camera_system")->set_active(true);
		engine::get()->get_subsystem("renderer")->set_visible(true);
	}

	void on_update(float delta_time)
	{
		//std::cout << "**************** MAIN GAME RUNNING *****************" << std::endl;
	}

	void initialise();
	void on_reset() {}
	void on_exit() {}
};