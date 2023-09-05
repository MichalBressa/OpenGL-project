#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"
#include "GameObject.h"

using namespace glm;

class Sphere : public GameObject
{
private:
	VertexWtihNormal* sphereVerticesNor;  //Sphere vertices data with normals
	unsigned int* sphereIndices;          //Sphere triangle indices    

	int stacks; // nunber of stacks
	int slices; // number of slices
	float radius;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO; //for triangle indices buffer

	mat4 ModelMatrix;

	void CreateSpherewithNormal(); //The function creates vertex data and normals 
public:
	Sphere();
	~Sphere();

	void SetPosition(vec3 newPos); //set the 3D position of the sphere
	vec3 GetPosition(void);        //get the 3D position of the sphere    

	void SetIDs(unsigned int, unsigned int, unsigned int); // (VAO, VBO, IBO) 
	void updateModelMatrix(unsigned int, float);           //(unsigned int modelViewMatLoc,float d)

	void Setup();
	void Draw();
	void Update(float, glm::vec3 speed);
};

