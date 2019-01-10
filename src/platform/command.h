#pragma once

#include "entity.h"

using namespace glm;

struct command
{
	virtual ~command() {}
	virtual void execute(std::shared_ptr<entity> actor) = 0;
};

struct move_command : public command
{
	vec3 direction;
	virtual void execute(std::shared_ptr<entity> actor);

	move_command(vec3& dir);
};