#include "input_handler.h"
#include "entity_manager.h"
#include <fstream>
#include "engine.h"

input_handler::input_handler()
{
	_visible = false;
	auto q_ = queue<shared_ptr<command>>();
	_data = make_shared<queue<shared_ptr<command>>> (q_);
}

shared_ptr<input_component> input_handler::build_component(shared_ptr<entity> e)
{
	// create input component and pass in the handler instance
	return make_shared<input_component>(e, _data);
}

void input_handler::load_input_settings()
{
	glfw_button_left = GLFW_KEY_A;
	glfw_button_right = GLFW_KEY_D;
	glfw_button_forward = GLFW_KEY_W;
	glfw_button_backward = GLFW_KEY_S;
}

bool input_handler::initialise()
{
	cout << "Input handler initialising" << endl;

	//map buttons
	load_input_settings();

	auto back = glm::vec3(0.0f, 0.0f, 1.0f);
	buttonDown_ = make_shared<move_command>(move_command(back));
	auto left = glm::vec3(-1.0f, 0.0f, 0.0f);
	buttonLeft_ = make_shared<move_command>(move_command(left));
	auto right = glm::vec3(1.0f, 0.0f, 0.0f);
	buttonRight_ = make_shared<move_command>(move_command(right));
	auto forward = glm::vec3(0.0f, 0.0f, -1.0f);
	buttonUp_ = make_shared<move_command>(move_command(forward));

	return true;
}

bool input_handler::load_content()
{
	cout << "Input handler loading content" << endl;
	return true;
}

void input_handler::update(float delta_time)
{
	//vector<shared_ptr<command>> commands;

	auto window = engine::get()->window;

	if (glfwGetKey(window, glfw_button_forward)) {

		cout << "Ahhhhhhh" << endl;
		_data->push(buttonUp_);
	}

	if (glfwGetKey(window, glfw_button_backward))
		_data->push(buttonDown_);

    if (glfwGetKey(window, glfw_button_left))
		_data->push(buttonLeft_);

    if (glfwGetKey(window, glfw_button_right))
		_data->push(buttonRight_);
}

void input_handler::render()
{
	// This should never be called.
	cout << "Input handler rendering" << endl;
}

void input_handler::unload_content()
{
	cout << "Input handler unloading content" << endl;
}

void input_handler::shutdown()
{
	cout << "Input handler shutting down" << endl;
}