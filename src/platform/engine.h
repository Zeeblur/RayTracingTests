#pragma once

#include <memory>
#include <unordered_map>
#include "vulkan_platform.h"

// forward declare subsystem
class subsystem;

class engine
{
private:
	bool _running = true;
	std::unordered_map<std::string, std::shared_ptr<subsystem>> _subsystems;
	engine() = default;

public:
	
	inline static std::shared_ptr<engine> get()
	{
		static std::shared_ptr<engine> instance(new engine());
		return instance;
	}

	GLFWwindow* window; // store a pointer to the glfw instance

	bool get_running() const noexcept { return _running; }
	void set_running(bool value) noexcept { _running = value; }

	void add_subsystem(const std::string &name, std::shared_ptr<subsystem> sys);

	std::shared_ptr<subsystem> get_subsystem(const std::string &name);

	void run();

	void init_subsystem();
	void main_loop();
	void cleanup();
};