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


void World::GravitySystem()
{
    float oldTimeSinceStart = 0.0f;
    

    for (
        auto [entity, gravity, transform] :
        registry.view<Gravity, Transform>().each()
        )
    {
        
        float timeSinceStart = static_cast<float>(glfwGetTime());
        gravity.deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        gravity.velocity += gravity.Gravitational_Constant * gravity.deltaTime;
        transform.posY += gravity.velocity * gravity.deltaTime;

        //dampen gravity with air resistance

        //drag = -b (cross sectional of area + density of fluid) * current velocity value but clamped to max gravity value
        //simulate terminal velocity as a max value alongside with drag
    
    }
}

void World::CollisionSystem()
{

}



void World::Update()
{
    CollisionSystem();
    GravitySystem();
   
}
