#include "camera_system.h"
#include <iostream>


bool camera_system::initialise() { return true; };

bool camera_system::load_content()
{
	std::cout << "Camera system loading content" << std::endl;
	return true;
}

void camera_system::update(float delta_time)
{
	// update view matrix here.
	for (auto &c : _data)
	{
		c->_view = glm::lookAt(c->_position, c->_target, c->_up);
		c->_projection = glm::perspective(c->fov, c->aspect, c->near, c->far);
	}
}

// This should never be called as _visible is false
void camera_system::render() {	}//std::cout << "Camera system rendering" << std::endl; }

void camera_system::unload_content() { std::cout << "Camera system unloading content" << std::endl; }
void camera_system::shutdown(){	std::cout << "Camera system shutting down" << std::endl; }