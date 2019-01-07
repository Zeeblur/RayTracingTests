#include "camera.h"
#include <glm/gtx/euler_angles.hpp>

camera_component::camera_component(std::shared_ptr<entity> &e, std::shared_ptr<camera_data> data)
	: _parent(e), _data(data) {}

void camera_component::update(float delta_time)
{
	// TODO: if moveable camera
	// get rotation and movement from entity.
	auto transforms = _parent->get_trans();
	
	// Calculate the forward direction - spherical coordinates to Cartesian
	glm::vec3 forward(cosf(transforms._pitch) * -sinf(transforms._yaw), sinf(transforms._pitch), -cosf(transforms._yaw) * cosf(transforms._pitch));
	forward = glm::normalize(forward);

	// Calculate standard right.  Rotate right vector by yaw
	glm::vec3 right = glm::vec3(glm::eulerAngleY(transforms._yaw) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	right = glm::normalize(right);

	// Up vector is up rotated by pitch
	_up = glm::cross(right, forward);
	_up = glm::normalize(_up);

	//// update position
	/// can do this if transform component sends message
	//glm::vec3 trans = _translation.x * right;
	//trans += _translation.y * _up;
	//trans += _translation.z * forward;
	//_position += trans;

	_target = _position + forward;

	// set data in struct
	_data->_up = _up;
	_data->_target = _target;
	_data->_position = _position;
}

bool camera_component::initialise(){ return true; }
bool camera_component::load_content() { return true; }
void camera_component::render() {} // should never be called
void camera_component::unload_content() {}
void camera_component::shutdown() {}
