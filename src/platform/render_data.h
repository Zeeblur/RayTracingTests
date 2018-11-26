#pragma once

#include <string>
#include <glm/glm.hpp>

struct render_data
{
	bool visible;
	std::string shader;
	glm::vec3 cam_pos;
	glm::mat4 M;
	glm::mat4 MV;
	glm::mat3 N;
	glm::mat4 MVP;
	glm::vec3 position;

	std::string parent_name;

	// Reference to structs
	//std::shared_ptr<gl::Effect> effect;

	// shape is now mesh
	//std::shared_ptr<gl::mesh_geom> mesh;

	// Reference to texture object
	//std::shared_ptr<texture> textureObj;

	// reference to material obj
	//gl::material_data matData;

	bool operator > (const render_data& other) const
	{
		return (M.length > other.M.length);
	}
};