#include "World.h"
#include <GLFW/glfw3.h>
#include "Entity.h"
#include <stdio.h>


World::World()
{

}

World::~World()
{

}

Entity World::create_entity(std::string tag)
{
    
    Entity entity(registry.create(), this);
    entity.add_component<TagComponent>(tag);
    return entity;
	
}


void World::GravitySystem(float deltaTime)
{
    
    for (
        auto [entity, gravity, transform] :
        registry.view<Gravity, Transform>().each()
        )
    {
        
        gravity.velocity += gravity.Gravitational_Constant * deltaTime;
        transform.posY += gravity.velocity * deltaTime;

        //dampen gravity with air resistance

        //drag = -b (cross sectional of area + density of fluid) * current velocity value but clamped to max gravity value
        //simulate terminal velocity as a max value alongside with drag
    
    }
}

void World::CollisionSystem()
{
    for (
        auto [entity, gravity, transform] :
        registry.view<Gravity, Transform>().each()
        )
    {
        //AABB COLLISION
    }

}



void World::Update(float deltaTime)
{
    CollisionSystem();
    GravitySystem(deltaTime);
   
}
