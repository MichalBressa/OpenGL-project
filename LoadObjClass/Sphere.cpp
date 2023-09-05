#include "Sphere.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

Sphere::Sphere()
{
	stacks = 10; //number of stacks
	slices = 10; //number of slices
	radius = 2.0f;
	position = vec3(-2, 1 ,0);

	VAO = VBO = 0;

	sphereVerticesNor = (VertexWtihNormal*)malloc(sizeof(VertexWtihNormal) * 121); //total number of vertices = (stacks+1)*(slices +1)
	sphereIndices = (unsigned int*)malloc(sizeof(unsigned int) * 660);
	CreateSpherewithNormal();
}

Sphere::~Sphere()
{
	free(sphereVerticesNor);
	free(sphereIndices);
}

void Sphere::CreateSpherewithNormal(void)
{
	int count;
	count = 0;
	for (int i = 0; i <= stacks; ++i) {

		GLfloat V = i / (float)stacks;
		GLfloat phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j) {

			GLfloat U = j / (float)slices;  //The parameter along all stacks, it range from 0 to 1
			GLfloat theta = U * (glm::pi <float>() * 2);  // The phi angle, it was converted to radian 

			// Calc The Vertex Positions
			GLfloat x = cosf(theta) * sinf(phi);
			GLfloat y = cosf(phi);
			GLfloat z = sinf(theta) * sinf(phi);

			sphereVerticesNor[count].coords = vec4(x * radius, y * radius + 6.0, z * radius, 1.0); //6.0 is just small tweak, you can remove it 
			sphereVerticesNor[count].normals = vec3(x, y, z); ///Sphere normals

			count++;
		}
	}

	count = 0;
	// Calc The Index Positions
	for (int i = 0; i < slices * stacks + slices; ++i) {

		//The triangle index is explained in the lecture slides
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i + slices;
		count++;

		sphereIndices[count] = i + slices + 1;
		count++;
		sphereIndices[count] = i;
		count++;
		sphereIndices[count] = i + 1;
		count++;
	}

	count = 0;
}

void Sphere::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 Sphere::GetPosition(void)
{
	return position;
}

void Sphere::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void Sphere::Setup()
{
	int verCount = 121;
	int triCount = 660;
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal) * verCount, sphereVerticesNor, GL_STATIC_DRAW);  
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triCount, sphereIndices, GL_STATIC_DRAW); 
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);  
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].coords));
   glEnableVertexAttribArray(3);
}

void Sphere::updateModelMatrix(unsigned int modelViewMatLoc,float d)
{
	ModelMatrix = mat4(1.0);
	ModelMatrix = lookAt(vec3(0.0, 14.0, 20.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
	ModelMatrix = glm::translate(ModelMatrix, position); //apply Sphere Position
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader
}

void Sphere::Draw()
{
	int triCount = 660;
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);
}

void Sphere::Update(float deltaTime, glm::vec3 speed)
{
	/*physicsBody->ResetTotalForces();

	float amount = 1.f * speed;

	if (GameObject::specialKeys[GLUT_KEY_UP] == true)
		physicsBody->AddForce(glm::vec3(0, 0, amount));
	if (GameObject::specialKeys[GLUT_KEY_DOWN] == true)
		physicsBody->AddForce(glm::vec3(0, 0, -amount));

	if (GameObject::specialKeys[GLUT_KEY_LEFT] == true)
		physicsBody->AddForce(glm::vec3(amount, 0, 0));
	if (GameObject::specialKeys[GLUT_KEY_RIGHT] == true)
		physicsBody->AddForce(glm::vec3(-amount, 0, 0));

	physicsBody->UpdateBody(deltaTime);*/
}