#pragma once
#include <iostream>
#include "entity_manager.h"
#include "engine.h"
#include "camera_system.h"

enum state_type
{
	START
};

class engine_state
{

public:
	state_type type;

	virtual ~engine_state() = default;

	virtual void initialise() = 0;

	virtual void on_reset() = 0;

	virtual void on_enter() = 0;

	virtual void on_update(float delta_time) = 0;

	virtual void on_exit() = 0;
};


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
		std::cout << "**************** MAIN GAME RUNNING *****************" << std::endl;
	}

	void initialise() {}
	void on_reset() {}
	void on_exit() {}
};