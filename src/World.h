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

		void Update(float dt);
		
		void GravitySystem(float dt);

		void CollisionSystem();

		void ProjectileMotionSystem(float dt);

		friend class Entity;


};