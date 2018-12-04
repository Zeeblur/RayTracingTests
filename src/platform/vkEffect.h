#pragma once

#include <string>

// Effect struct
struct vkEffect
{
	std::string name;
	bool has_geometry;
	bool is_compute;
	void *GpuData;
	//GLuint program;

	//std::vector<std::shared_ptr<light_data>> lights;
};


struct texture
{
	uint32_t _type;
	uint32_t _id;

	// Set attributes
	uint32_t _height;
	uint32_t _width;

	std::string _filename;

	texture(std::string filename) {};

};

//texture::texture(std::string filename)
//{
//	int texWidth, texHeight, texChannels;
//
//
////	unsigned char* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
////
////
////	if (!pixels)
////		throw std::runtime_error("failed to load texture image!");
////
////	// Generate texture with OpenGL
////	glGenTextures(1, &_id);
////	glBindTexture(GL_TEXTURE_2D, _id);
////
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
////
////	// Check for any errors with OpenGL
////	if (CHECK_GL_ERROR)
////	{
////		// Problem creating texture object
////		std::cerr << "ERROR - loading texture " << filename << std::endl;
////		std::cerr << "Could not allocate texture with OpenGL" << std::endl;
////		// Unload FreeImage data
////		stbi_image_free(pixels);
////		// Set id to 0
////		_id = 0;
////		// Throw exception
////		throw std::runtime_error("Error creating texture");
////	}
////
////	// Turn on linear mipmaps
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	CHECK_GL_ERROR; // Not considered fatal here
////
////// Now set texture data
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
////	// Check if error
////	if (CHECK_GL_ERROR)
////	{
////		// Error loading texture data into OpenGL
////		std::cerr << "ERROR - loading texture " << filename << std::endl;
////		std::cerr << "Could not load texture data in OpenGL" << std::endl;
////		// Unload FreeImage data
////		stbi_image_free(pixels);
////		// Unallocate image with OpenGL
////		glDeleteTextures(1, &_id);
////		_id = 0;
////		// Throw exception
////		throw std::runtime_error("Error creating texture");
////	}
////
////
////	glGenerateMipmap(GL_TEXTURE_2D);
////
////	// Set attributes
////	_height = texHeight;
////	_width = texWidth;
////	_type = GL_TEXTURE_2D;
////
////	// Unload image data
////	stbi_image_free(pixels);
////	CHECK_GL_ERROR; // Non-fatal - just info
////
////					// Log
////	std::clog << "LOG - texture " << filename << " loaded" << std::endl;
////
////	glBindTexture(GL_TEXTURE_2D, 0);
//};