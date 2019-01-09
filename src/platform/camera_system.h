#pragma once
#include "subsystem.h"
#include <vector>
#include "camera.h"

class camera_system : public subsystem
{
private:
	std::vector<std::shared_ptr<camera_data>> _data;
	size_t current_id = 0;
public:

	static inline std::shared_ptr<camera_system> get()
	{
		static std::shared_ptr<camera_system> instance = std::shared_ptr<camera_system>(new camera_system());
		return instance;
	}

	std::shared_ptr<camera_component> build_component(std::shared_ptr<entity> e, Camera_Type type);

	bool initialise() override;
	bool load_content() override;
	void update(float delta_time) override;
	void render() override;
	void unload_content() override;
	void shutdown() override;

	inline camera_data& get_cam() { return *_data[current_id]; }
};