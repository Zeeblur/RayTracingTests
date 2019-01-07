#pragma once
#include <memory>

class subsystem
{
protected:
	bool _active = true;
	bool _visible = true;

public:

	subsystem() = default;
	subsystem(const subsystem &other) = default;
	subsystem(subsystem &&rhs) = default;
	subsystem& operator=(const subsystem &other) = default;
	subsystem& operator=(subsystem &&rhs) = default;
	virtual ~subsystem() = default;


	bool get_active() const noexcept { return _active; }
	bool get_visible() const noexcept { return _visible; }

	void set_active(bool active) noexcept { _active = active; }
	void set_visible(bool visible) noexcept { _visible = visible; }


	virtual bool initialise() = 0;
	virtual bool load_content() = 0;
	virtual void update(float delta_time) = 0;
	virtual void render() = 0;
	virtual void unload_content() = 0;
	virtual void shutdown() = 0;
};