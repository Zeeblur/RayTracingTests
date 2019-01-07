#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class camera
{
public:
	camera() = default;
	virtual ~camera() = 0;


	const vec3& get_position() const { return _position; }
	void set_position(const glm::vec3 &value) { _position = value; }
	const vec3& get_target() const { return _target; }
	void set_target(const glm::vec3 &value) { _target = value; }
	const vec3& get_up() const { return _up; }
	void set_up(const glm::vec3 &value) { _up = value; }

	const glm::mat4& get_view() const { return _view; }

	glm::mat4 get_projection() const { return _projection; }
	void set_projection(float fov, float aspect, float near, float far){ _projection = perspective(fov, aspect, near, far); }
	
	// update camera does nothing
	virtual void update(float delta_time) = 0;

protected:
	vec3 _position = vec3(0.0f);
	vec3 _target = vec3(0.0f, 0.0f, -1.0f);
	vec3 _up = vec3(0.0f, 1.0f, 0.0f);
	mat4 _view;
	mat4 _projection;
};

class free_cam : public camera
{
private:
	float _pitch = 0.0f; // rotation on the x-axis 
	float _yaw = 0.0f; // rotation on the y-axis
	glm::vec3 _translation;

public:

	free_cam() : camera() { }
	~free_cam() { }

	void update(float delta_time);


	float get_pitch() const { return _pitch; }
	float get_yaw() const { return _yaw; }

	void set_pitch(float value) { _pitch = value; }
	void set_yaw(float value) { _yaw = value; }

	void rotate(float delta_yaw, float delta_pitch); 	// Rotate by the given yaw (y-axis) and pitch (x-axis)
	void move(const glm::vec3 &translation); 			// Move the cam
};
