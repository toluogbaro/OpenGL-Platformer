#include "Entity.h"

Entity::Entity(entt::entity s_entity_handle, World* s_world)
	: entity_handle(s_entity_handle), world(s_world)
{

}

Entity::~Entity()
{

}