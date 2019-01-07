#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "component.h"
#include "entity.h"

using namespace glm;

enum Camera_Type { STATIC, FREE };

struct camera_data
{
	float fov;
	float aspect;
	float near;
	float far;
	float left = 0;
	float bottom = 0;

	vec3 _position;
	vec3 _target;
	vec3 _up;

	Camera_Type _type;
	mat4 _view;
	mat4 _projection;
};

class camera_component : public component
{
private:
	std::shared_ptr<camera_data> _data;
	std::shared_ptr<entity> _parent;

	vec3 _position = vec3(0.0f);
	vec3 _target = vec3(0.0f, 0.0f, -1.0f);
	vec3 _up = vec3(0.0f, 1.0f, 0.0f);

public:
	camera_component(std::shared_ptr<entity> &e, std::shared_ptr<camera_data> data);

	bool initialise() override final;
	bool load_content() override final;
	void update(float delta_time) override final;
	void render() override final;
	void unload_content() override final;
	void shutdown() override final;

	const vec3& get_position() const { return _position; }
	const vec3& get_target() const { return _target; }
	const vec3& get_up() const { return _up; }

	std::shared_ptr<camera_data> get_data() const { return _data; }
};
