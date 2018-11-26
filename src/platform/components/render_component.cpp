#define GLM_ENABLE_EXPERIMENTAL
#include "render_component.h"
#include <sstream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera_component.h"
#include "../systems/camera_system.h"
#include "../systems/renderer.h"

using namespace glm;

render_component::render_component(std::shared_ptr<entity> e, std::shared_ptr<render_data> data)
	: _parent(e), _data(data)
{
	_active = false;
	_data->visible = true;
}

std::shared_ptr<render_data> render_component::get_data()
{
	return _data;
}

bool render_component::initialise()
{
	//programID = gl::LoadShaders("res/shaders/simple.vert", "res/shaders/simple.frag");
	return true;
}

bool render_component::load_content()
{
	// create vbo
	return true;
}

void render_component::update(float delta_time)
{
	// This should never be called.
}

void render_component::render()
{
 //   _data->visible = get_visible();

	//if (_data->visible)
	//{
	//	// "Generate" the transform matrix.
	//	vec3 transvec = vec3(_parent->get_trans().x, _parent->get_trans().y, _parent->get_trans().z);
	//	_data->position = transvec;
	//	//std::cout << "rendering the: " + _parent->get_name() << " at (" << transvec.x << ", " << transvec.y << ", " << transvec.z << ")" << std::endl;
	//	_data->parent_name = _parent->get_name();
	//	mat4 trans = glm::translate(mat4(1.0f), transvec);

	//	mat4 rotation;

	//	glm::quat rot = _parent->get_trans().rotation;
	//	glm::vec3 scal = _parent->get_trans().scale;


	//	rotation = mat4_cast(rot);

	//	mat4 scale = glm::scale(mat4(1.0f), scal);

	//	mat4 model = trans * (rotation * scale);

	//	auto camera = static_cast<camera_component*>(_parent->get_component("camera").get());

	//	int width, height;
	//	glfwGetWindowSize(glfw::window, &width, &height);
	//	mat4 proj;
	//	mat4 view;
	//	vec3 eye_pos;

	//	if (camera != nullptr)
	//	{
	//		if (camera->get_data()->type == camera_type::CHASE) {
	//			proj = camera->get_projection();
	//			view = camera->get_view();
	//		}
	//		else
	//		{
	//			proj = glm::ortho(-0.5f * (float)width, 0.5f * (float)width, -0.5f * (float)height, 0.5f * (float)height, -1000.0f, 1000.0f);
	//			view = mat4(1.0f);
	//		}

	//		eye_pos = camera->get_data()->cam_pos;
	//	}
	//	else
	//	{
	//		proj = camera_system::get()->player_cam_P;
	//		view = camera_system::get()->player_cam_V;
	//		eye_pos = camera_system::get()->player_cam_pos;
	//	}

	//	auto MVP = proj * view * model;

	//	
	//	// create render job here
	//	_data->MV = view * model;
	//	_data->MVP = MVP;
	//	_data->cam_pos = eye_pos;
	//	_data->N = rotation;

	//	//if (_data->effect->name == "text")
	//	//{
	//	//	renderer::get()->_dataText.push_back(_data);
	//	//}
	//	//else
	//	//{
	//	//	renderer::get()->_dataList.push_back(_data);
	//	//}
		
	//}
}

void render_component::unload_content()
{

}

void render_component::shutdown()
{

}