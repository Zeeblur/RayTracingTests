#pragma once

#include <iostream>
#include <memory>
#include "subsystem.h"
#include "input_component.h"
#include "command.h"

using namespace std;

class input_handler : public subsystem
{
private:
	input_handler();

	shared_ptr<queue<shared_ptr<command>>> _data;

	// button references
	shared_ptr<command> buttonUp_;
	shared_ptr<command> buttonDown_;
	shared_ptr<command> buttonLeft_;
	shared_ptr<command> buttonRight_;


public:
	int glfw_button_left;
	int glfw_button_right;
	int glfw_button_forward;
	int glfw_button_backward;

	inline static shared_ptr<input_handler> get()
	{
		static shared_ptr<input_handler> instance(new input_handler());
		return instance;
	}

	shared_ptr<input_component> build_component(shared_ptr<entity> e);

	void load_input_settings();

	bool initialise();
	bool load_content();
	void update(float delta_time);
	void render();
	void unload_content();
	void shutdown();
};