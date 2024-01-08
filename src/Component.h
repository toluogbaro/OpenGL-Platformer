#pragma once

#include <string>


struct TagComponent
{
	std::string tag;
};

struct Transform
{
	float posX;
	float posY;
	float posZ;

	float yaw;
	float pitch;
	float scroll;

	float scaleX;
	float scaleY;
	float scaleZ;

};

struct VelocityComponent
{

};

struct Collision
{
	
};

struct Gravity
{

	//acceleration
	const float Gravitational_Constant = -9.81f;
	float deltaTime;
	float velocity;
	float weight;

	//drag : -b is the cross sectional area and density of the fluid that the object is going through
	float b;
	
};