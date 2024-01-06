#include "World.h"
#include <GLFW/glfw3.h>


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


void World::Gravity()
{
    float oldTimeSinceStart = 0.0f;
    
    
    for (
        auto [entity, grav, transform] :
        registry.view<GravityComponent, Transform>().each()
        )
    {
        float timeSinceStart = static_cast<float>(glfwGetTime());
        grav.deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        grav.velocity = 0;
        grav.velocity += grav.Gravitational_Constant * grav.deltaTime;
        transform.posY += grav.velocity * grav.deltaTime;
    
    }
}

void World::Update()
{
    Gravity();
}
