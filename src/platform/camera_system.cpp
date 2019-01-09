#include "camera_system.h"
#include <iostream>
#include "vulkan_platform.h"

std::shared_ptr<camera_component> camera_system::build_component(std::shared_ptr<entity> e, Camera_Type type)
{
	camera_data proj;
	int width = 0, height = 0;
	vulkan_platform::get()->get_window_dims(&width, &height);

	// switch on type here
	proj.fov = 1.0472f;  // 60 degrees
	proj.aspect = width/height;
	proj.near = 0.1f;
	proj.far = 10.0f;
	proj.left = -0.5f * (float)width;
	proj.bottom = -0.5f * (float)height;
	proj._type = FREE;

	_data.push_back(std::make_shared<camera_data>(proj));
	return std::make_shared<camera_component>(e, std::ref(_data.back()));
}

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