#pragma once
#include "entt/entt.hpp"
#include "Component.h"
#include "Entity.h"


class Entity;

class World
{

	private:

		entt::registry registry;

	public:

		World();
		~World();

		Entity create_entity(std::string tag);

		void Update();
		
		void Gravity();

		friend class Entity;


};