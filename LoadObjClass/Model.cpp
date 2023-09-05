#include "Model.h"
#include "OBJloader.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

Model::Model(const char* name)
{
	position = vec3(0);
	VAO = VBO = 0;
	VerticesData = NULL;
	NumVert = 0;
	CreateObject(name);
}


Model::~Model()
{
	free(VerticesData);
}

void Model::CreateObject(const char* name)
{
	std::vector<VertexWithAll> mesh = loadOBJ(name); //Import OBJ file data into a vector array
	NumVert = mesh.size();

	this->VerticesData = new VertexWithAll[NumVert];
	for (size_t i = 0; i < NumVert; i++)
	{
		this->VerticesData[i] = mesh[i];
	}
}


void Model::SetPosition(vec3 newPos)
{
	position = newPos;
}

vec3 Model::GetPosition(void)
{
	return position;
}

void Model::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}


	/*int verCount = 121;
	int triCount = 660;
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWtihNormal) * verCount, sphereVerticesNor, GL_STATIC_DRAW);  ///please note the change
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triCount, sphereIndices, GL_STATIC_DRAW); ///please note the change
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);  //layout(location=4) in vec4 fieldCoords;
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].normals));
   glEnableVertexAttribArray(3);*/

void Model::Setup()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithAll) * NumVert, VerticesData, GL_STATIC_DRAW);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, position));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, normal));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, textcoord));
	glEnableVertexAttribArray(6);

}

	/*ModelMatrix = mat4(1.0);
	ModelMatrix = lookAt(vec3(0.0, 10.0, 15.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
	ModelMatrix = glm::translate(ModelMatrix, position); //apply Sphere Position
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix));  //send modelview matrix to the shader*/

void Model::updateModelMatrix(unsigned int modelViewMatLoc, float d, float scale, vec3 offsetPos)
{
	ModelMatrix = mat4(1.0);
	ModelMatrix = lookAt(vec3(0.0, 14.0, 20.0), vec3(0.0 + d, 10.0, 0.0), vec3(0.0, 1.0, 0.0)); //camera matrix, apply first
	ModelMatrix = glm::scale(ModelMatrix, vec3(scale, scale, scale)); //scale down the model
	ModelMatrix = glm::translate(ModelMatrix, offsetPos);
	ModelMatrix = glm::translate(ModelMatrix, GetPosition());
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(ModelMatrix)); //send modelview matrix to the shader
}


	/*int triCount = 660;
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);*/

void Model::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, NumVert);
}



void Model::Update(float deltaTime, glm::vec3 offset)
{
	//collider->Update(deltaTime, position, offset);
}