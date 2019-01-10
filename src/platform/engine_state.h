#pragma once
#include <iostream>
#include "entity_manager.h"
#include "camera_system.h"
#include "physics_system.h"
#include "input_handler.h"
#include "engine.h"

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