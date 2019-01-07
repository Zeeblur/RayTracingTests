#include "camera.h"
#include <glm/gtx/euler_angles.hpp>


void free_cam::update(float delta_time)
{
	// Calculate the forward direction - spherical coordinates to Cartesian
	glm::vec3 forward(cosf(_pitch) * -sinf(_yaw), sinf(_pitch), -cosf(_yaw) * cosf(_pitch));
	forward = glm::normalize(forward);

	// Calculate standard right.  Rotate right vector by yaw
	glm::vec3 right = glm::vec3(glm::eulerAngleY(_yaw) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	right = glm::normalize(right);

	// Up vector is up rotated by pitch
	_up = glm::cross(right, forward);
	_up = glm::normalize(_up);

	// update position
	glm::vec3 trans = _translation.x * right;
	trans += _translation.y * _up;
	trans += _translation.z * forward;
	_position += trans;

	_target = _position + forward;

	// reset translation to zero
	_translation = glm::vec3(0.0f, 0.0f, 0.0f);
	
	// update view matrix
	_view = glm::lookAt(_position, _target, _up);
}


void free_cam::rotate(float delta_yaw, float delta_pitch)
{
	// calculate new orientation
	_pitch += delta_pitch;
	_yaw -= delta_yaw;
}

void free_cam::move(const glm::vec3 &translation)
{
	// calculate new translation
	_translation += translation;
}