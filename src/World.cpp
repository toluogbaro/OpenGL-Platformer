#include "World.h"
#include <GLFW/glfw3.h>
#include "Entity.h"
#include <stdio.h>
#include "glm/gtc/matrix_transform.hpp"


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


void World::GravitySystem(float dt)
{
    
    for (
        auto [entity, gravity, transform] :
        registry.view<Gravity, Transform>().each()
        )
    {
        
        gravity.velocity += gravity.Gravitational_Constant * dt;
        transform.posY += gravity.velocity * dt;

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

void World::ProjectileMotionSystem(float dt)
{

    for (
        auto [entity, kinematic, transform] :
        registry.view<Kinematic, Transform>().each()
        )
    {
        float xVector = glm::cos(kinematic.angle * (3.14f / 180.0f) * kinematic.power);
        float yVector = glm::sin(kinematic.angle * (3.14f / 180.0f) * kinematic.power);

        float finalPosX = (kinematic.accelerationX / 2.0f) * (dt * dt) + (kinematic.velocityX * dt) + xVector;
        float finalPosY = (kinematic.accelerationY / 2.0f) * (dt * dt) + (kinematic.velocityY * dt) + yVector;

        transform.posX += finalPosX;
        transform.posY += finalPosY;


        //angles not working right, clean it up
    }

}



void World::Update(float deltaTime)
{
    //CollisionSystem();
    GravitySystem(deltaTime);
    //ProjectileMotionSystem(deltaTime);
   
}
