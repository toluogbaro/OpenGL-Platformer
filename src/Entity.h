#pragma once

#include "World.h"

class Entity
{
	private:
		entt::entity entity_handle = entt::null;
		World* world = NULL;
		float m_Mass;

	public:
		Entity(entt::entity s_entity_handle, World* s_world);

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