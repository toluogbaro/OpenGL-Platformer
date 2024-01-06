#pragma once

struct TagComponent
{

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

struct AABBCollisionComponent
{

};

struct GravityComponent
{
	const float Gravitational_Constant = -9.81f;
	float deltaTime;
	float velocity;
	float weight;
	
};