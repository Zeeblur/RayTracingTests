#pragma once

#include <memory>
#include "subsystem.h"

// contains everything needed for a vulkan rendering platform.
class vulkan_platform : public subsystem
{
public:
	inline static std::shared_ptr<vulkan_platform> get()
	{
		std::shared_ptr<vulkan_platform> instance(new vulkan_platform);
		return instance;
	}

	bool initialise();
	bool load_content();
	void update(float delta_time);
	void render();
	void unload_content();
	void shutdown();
};
