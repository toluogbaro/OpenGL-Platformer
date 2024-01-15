#pragma once

//Credit to https://youtube.com/@danielblagy for the Entity Component System Tutorial
//Followed Tutorial: https://youtu.be/8LbVpkEqKuY?feature=shared

#include "entt/entt.hpp"
#include "World.h"

class Entity
{
	private:
		
		World* world = NULL;

		entt::entity entity_handle = entt::null;

	public:

		float m_Mass;

		Entity(entt::entity s_entity_handle, World* s_world);

		~Entity();

		inline entt::entity get_handle() { return entity_handle; }

		template<typename T, typename... Args>
		T& add_component(Args&&... args)
		{
			return world->registry.emplace<T>(entity_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& get_component()
		{
			return world->registry.get<T>(entity_handle);
		}

		template<typename T>
		void remove_component()
		{
			return world->registry.remove<T>(entity_handle);
		}

		template<typename T>
		bool has_component()
		{
			return world->registry.all_of<T>(entity_handle);
		}
};