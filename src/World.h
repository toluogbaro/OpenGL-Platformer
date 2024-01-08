#pragma once
#include "entt/entt.hpp"
#include "Component.h"


class Entity;

class World
{

	private:

		entt::registry registry;

	public:

		World();
		~World();

		Entity create_entity(std::string tag);

		void Update(float deltaTime);
		
		void GravitySystem(float deltaTime);

		void CollisionSystem();

		friend class Entity;


};