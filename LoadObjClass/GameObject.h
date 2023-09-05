#pragma once

#include <map>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
//#include "SphereCollider.h" //link to your physics (Sphere collider)

class GameObject
{
protected:
	//physics stuff here
	//PhysicsBody* physicsBody;
	//SphereCollider* collider;


public:
	GameObject();
	GameObject(glm::vec3 pos);
	~GameObject();

	//3D position of the object
	glm::vec3 position;

	//Get sphere collider 
	//SphereCollider* GetCollider();

	//OpenGL drawing function
	virtual void Draw() = 0;

	///physics update function
	virtual void Update(float);

	//keyboard input
	static std::map<char, bool> keys;
	static std::map<char, bool> specialKeys;
};

