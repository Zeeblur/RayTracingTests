#include <ratio>
#include "physics_system.h"

using namespace glm;

physics_system::physics_system()
{
	_visible = false;

}

std::shared_ptr<physics_component> physics_system::build_component(std::shared_ptr<entity> e)
{

	auto pd = std::make_shared<physics_data>(physics_data(e->get_trans()));

	_data.push_back(std::ref(pd));
	return std::make_shared<physics_component>(e, pd);

}

std::shared_ptr<physics_component> physics_system::build_component(std::shared_ptr<entity> e, std::shared_ptr<physics_data> d)
{

	_data.push_back(d);
	return std::make_shared<physics_component>(e, d);
}

bool physics_system::initialise()
{
	std::cout << "Physics system initialising" << std::endl;
	return true;
}

bool physics_system::load_content()
{
	std::cout << "Physics system loading content" << std::endl;
	return true;
}

void physics_system::update(float delta_time)
{

	//std::cout << "Physics system updating" << std::endl;
	for (auto &d : _data)
	{
		//push back task to list.
		if (d->active)
		{
			// change by speed and delta-time.
			auto movement = d->currentVelocity * delta_time;

			// movement test here....
			d->x += movement.x;
			d->y += movement.y;
			d->z += movement.z;

			if (!d->moveRequest) // slow down
			{
				//std::cout << moveSpeed.x << ", " << moveSpeed.y << std::endl;
				// lateral movement
				if (d->currentVelocity.x < 0) d->currentVelocity.x += deceleration.x;
				if (d->currentVelocity.x > 0) d->currentVelocity.x -= deceleration.x;

				// if speed within epsilon of zero. Reset to zero
				if (d->currentVelocity.x > 0 && d->currentVelocity.x < deceleration.x) d->currentVelocity.x = 0;
				if (d->currentVelocity.x < 0 && d->currentVelocity.x > -deceleration.x) d->currentVelocity.x = 0;

				// vertical movement
				if (d->currentVelocity.y < 0) d->currentVelocity.y += deceleration.y;
				if (d->currentVelocity.y > 0) d->currentVelocity.y -= deceleration.y;

				// if speed within epsilon of zero. Reset to zero
				if (d->currentVelocity.y > 0 && d->currentVelocity.y < deceleration.y) d->currentVelocity.y = 0;
				if (d->currentVelocity.y < 0 && d->currentVelocity.y > -deceleration.y) d->currentVelocity.y = 0;

				// forwards movement
				if (d->currentVelocity.z < 0) d->currentVelocity.z += deceleration.z;
				if (d->currentVelocity.z > 0) d->currentVelocity.z -= deceleration.z;

				// if speed within epsilon of zero. Reset to zero
				if (d->currentVelocity.z > 0 && d->currentVelocity.z < deceleration.z) d->currentVelocity.z = 0;
				if (d->currentVelocity.z < 0 && d->currentVelocity.z > -deceleration.z) d->currentVelocity.z = 0;
			}

			// reset move request
			d->moveRequest = false;
		}
	}


}

void physics_system::render()
{
	// This should never be called.
	std::cout << "Physics system rendering" << std::endl;
}

void physics_system::unload_content()
{
	std::cout << "Physics system unloading content" << std::endl;
}

void physics_system::shutdown()
{
	std::cout << "Physics system shutting down" << std::endl;
}