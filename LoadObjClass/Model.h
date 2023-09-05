#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"
#include "GameObject.h"

using namespace glm;

class Model : public GameObject
{
private:
	VertexWithAll* VerticesData;  //Data with vertices, normal, texCoords

	int NumVert;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO; //for triangle indices buffer

	mat4 ModelMatrix;

	void CreateObject(const char*);

public:
	Model(const char*);

	~Model();

	void SetPosition(vec3 newPos);
	vec3 GetPosition(void);

	void SetIDs(unsigned int, unsigned int, unsigned int);
	void updateModelMatrix(unsigned int, float, float, vec3); //(MatrixLoc, camera offset,scale, Object Position);


	void Setup();
	void Draw();
	void Update(float, glm::vec3 offset);
};

