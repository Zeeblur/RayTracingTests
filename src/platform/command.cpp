#include "command.h"
#include "physics_component.h"

move_command::move_command(glm::vec3& dir) : direction(dir) {}

void move_command::execute(std::shared_ptr<entity> actor)
{
	// Move command here. to phys
	std::cout << "Move me " << actor->get_name() << std::endl;

	// need to re-add physics components later
	auto phys = static_cast<physics_component*>(actor->get_component("physics").get());

	if (phys != nullptr)
	{
		phys->add_impulse(direction);
	}

}