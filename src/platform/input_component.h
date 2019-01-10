#pragma once

#include "component.h"
#include "entity.h"
#include "command.h"
#include <queue>

using namespace std;

struct input_component : public component
{
private:
	// We'll also keep a reference to the parent entity
	shared_ptr<entity> _parent;

	// reference to QUEUE! of command data
	shared_ptr<queue<shared_ptr<command>>> _data;

public:
	input_component(shared_ptr<entity> &e, shared_ptr<queue<shared_ptr<command>>> &data);

	bool initialise() override final;

	bool load_content() override final;

	void update(float delta_time) override final;

	void render() override final;

	void unload_content() override final;

	void shutdown() override final;
};

