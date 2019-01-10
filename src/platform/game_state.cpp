#include "game_state.h"

void engine_state_1::initialise()
{
	transform_data actTrans;
	actTrans.z = -4.0f;
	auto actor = entity_manager::get()->create_entity("Actor", this->type, actTrans);
	actor->add_component("camera", camera_system::get()->build_component(actor, FREE));
	actor->add_component("physics", physics_system::get()->build_component(actor));
	actor->add_component("input", input_handler::get()->build_component(actor));
}