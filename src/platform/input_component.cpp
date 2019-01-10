#include "input_component.h"
#include "Command.h"
#include <vector>


input_component::input_component(shared_ptr<entity> &e, shared_ptr<queue<shared_ptr<command>>>	&data) : _parent(e), _data(data)
{
	_visible = false;
}

bool input_component::initialise()
{
	return true;
}

bool input_component::load_content()
{
	return true;
}

void input_component::update(float delta_time)
{
	// execute all commands
	while (!_data->empty())
	{
		auto com = _data->front();
		_data->pop();
		com->execute(_parent);
	}
}

void input_component::render()
{
}

void input_component::unload_content()
{
}

void input_component::shutdown()
{
}