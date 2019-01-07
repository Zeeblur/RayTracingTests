#include "engine_state_machine.h"
#include "entity_manager.h"

engine_state_machine::engine_state_machine()
{
	_visible = false;
}

std::shared_ptr<engine_state_machine> engine_state_machine::get()
{
	static std::shared_ptr<engine_state_machine> instance(new engine_state_machine());
	return instance;
}

void engine_state_machine::add_state(const std::string &name, std::shared_ptr<engine_state> state, state_type type)
{
	state->type = type;
	_states[name] = state;
}

void engine_state_machine::change_state(const std::string &name, bool reset)
{
	if (name != _current_state_name)
	{
		auto found = _states.find(name);
		if (found != _states.end())
		{
			if (_current_state != nullptr)
				_current_state->on_exit();
			_current_state = found->second;

			if (reset)
				_current_state->on_reset();

			_current_state->on_enter();
			_current_state_name = name;
			_current_state_type = _current_state->type;

			entity_manager::get()->currentState = _current_state_type;
		}
		else
		{
			throw std::invalid_argument("state not found");
		}
	}
}

std::string engine_state_machine::get_current_state()
{
	return _current_state_name;
}

state_type engine_state_machine::get_current_state_type()
{
	return _current_state_type;
}

bool engine_state_machine::initialise()
{
	for (auto &s : _states)
	{
		s.second->initialise();
	}
	return true;
}

void engine_state_machine::update(float delta_time)
{
	if (_current_state != nullptr)
		_current_state->on_update(delta_time);

}

bool engine_state_machine::load_content() { return true; }
void engine_state_machine::render() {}
void engine_state_machine::unload_content() {}
void engine_state_machine::shutdown() {}